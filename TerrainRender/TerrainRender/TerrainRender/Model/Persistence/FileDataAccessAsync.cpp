#include "DataAccess.h"
#include <fstream>
#include <string>
#include "../../win.h"
#include "ModelStructs.h"
#include <vector>
#include <thread>
#include <sstream>
#include <mutex>
#include <memory>
#include "../../ErrorHandler.h"
#include "from_json.h"

int BinaryFileDataAccessAsync::GetNumThreads(int numOfFacets)
{
    int maxNumThreads = std::thread::hardware_concurrency();
    int numThreads;
    
    numThreads = numOfFacets / MIN_NUM_OF_FACETS_IN_CHUNK;
    numThreads = max(1, numThreads);
    numThreads = min(maxNumThreads, numThreads);

    return numThreads;
}

unsigned BinaryFileDataAccessAsync::GetNumTriangles_stlBin(const std::wstring& filepath)
{
;
    std::ifstream file(filepath, std::ios::binary); // Open file and seek to end
    std::wstring errormsg = L"Failed to open " + std::wstring(filepath);
    THROW_TREXCEPTION_IF_FAILED(file.is_open(), errormsg);


    // Get the size of the file
    file.seekg(0, std::ios::end);
    unsigned fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read the number of triangles
    unsigned numTriangles;
    file.seekg(STL_BIN_HEADER_SIZE, std::ios_base::beg);
    file.read(reinterpret_cast<char*>(&numTriangles), sizeof(numTriangles));
    errormsg = L"Invalid binary STL file" + std::wstring(filepath);
    THROW_TREXCEPTION_IF_FAILED((fileSize == STL_BIN_HEADER_SIZE + STL_BIN_NUM_OF_TRIANGLE_SIZE + STL_BIN_TRIANGLE_SIZE * numTriangles), errormsg);

    file.close();

    return numTriangles;
}

void BinaryFileDataAccessAsync::ReadFileSharpEdges(const std::wstring& filepath)
{
    unsigned numTriangles = GetNumTriangles_stlBin(filepath);
    
    // Calculate how many threads are needed
    int numThreads = GetNumThreads(numTriangles);
    // Calculate how many facets are in a chunk
    int numFacetInChunk = numTriangles / numThreads;
    
    std::vector<std::thread> threads;
    std::vector<ICallablePtr> processes;
    std::vector<std::vector<StlFacet>> facetVectors(numThreads);
    
    for (int i = 0; i < numThreads; i++)
    {
        unsigned currentNumOfFacets;
        unsigned beginInBytes = STL_BIN_HEADER_SIZE + STL_BIN_NUM_OF_TRIANGLE_SIZE + STL_BIN_TRIANGLE_SIZE * numFacetInChunk * i;
        if (i < numThreads - 1)
            currentNumOfFacets = numFacetInChunk;
        else
            currentNumOfFacets = numTriangles - i * numFacetInChunk;
    
        processes.emplace_back(std::make_shared<ReadSTLChunkSharp>(filepath, beginInBytes, currentNumOfFacets, facetVectors.at(i)));
        threads.emplace_back(std::thread(std::ref(*processes.at(i))));
    }
    
    m_facets.clear();
    
    for (int i = 0; i < numThreads; i++)
    {
        if (threads.at(i).joinable())
            threads.at(i).join();
    }
    for (int i = 0; i < numThreads; i++)
    {
        m_facets.insert(m_facets.end(), facetVectors.at(i).begin(), facetVectors.at(i).end());
    }
}


const std::vector<StlVertex>& BinaryFileDataAccessAsync::GetVertices_Soft()
{
    return this->m_vertices;
}
const std::vector<CornerIndices>& BinaryFileDataAccessAsync::GetIndices_Soft()
{
    return this->m_indices;
}
void BinaryFileDataAccessAsync::ReadFileSoftEdges(const std::wstring& filepath)
{

   unsigned numTriangles = GetNumTriangles_stlBin(filepath);

   // Calculate how many threads are needed
   int numThreads = GetNumThreads(numTriangles);
   // Calculate how many facets are in a chunk
   int numOfFacetInChunk = numTriangles / numThreads;

   std::vector<std::thread> threads;
   std::vector<ICallablePtr> callables;
   std::vector<IndicesVecPtr> indicesVectors;
   std::vector<Map_Ind_NormalsPtr> mapVectors;    
   HashTable_Soft ht;

   size_t nextID = 0;

   for (int i = 0; i < numThreads; i++)
   {
       unsigned currentNumOfFacets;
       unsigned beginInBytes = STL_BIN_HEADER_SIZE + STL_BIN_NUM_OF_TRIANGLE_SIZE + STL_BIN_TRIANGLE_SIZE * numOfFacetInChunk * i;
       if (i < numThreads - 1)
           currentNumOfFacets = numOfFacetInChunk;
       else
           currentNumOfFacets = numTriangles - i * numOfFacetInChunk;

       IndicesVecPtr indicesVecPtr = std::make_shared<IndicesVec>();
       indicesVectors.push_back(indicesVecPtr);

       Map_Ind_NormalsPtr mapPtr = std::make_shared<Map_Ind_Normals>();
       mapVectors.push_back(mapPtr);

       callables.emplace_back(std::make_shared<ReadSTLChunkSoft>(filepath, beginInBytes, currentNumOfFacets, indicesVecPtr, ht, m_mutex_hashtable, nextID, mapPtr));
       threads.emplace_back(std::thread(std::ref(*callables.at(i))));
   }
   for (int i = 0; i < numThreads; i++)
   {
       if (threads.at(i).joinable())
           threads.at(i).join();
   }

   m_vertices.clear();
   m_indices.clear();

   //Collect Indices
   for (IndicesVecPtr indicesVec : indicesVectors)
   {
       m_indices.insert(m_indices.begin(), indicesVec->begin(), indicesVec->end());
   }

   size_t vectorSize = min(nextID, ht.size());

   std::vector<Vector3D> normals(vectorSize);

   //Collect and Calculate vertex normal vectors
   for (Map_Ind_NormalsPtr map : mapVectors)
   {
       for (auto& pair : *map.get())
       {
           const size_t& index = pair.first;

           if (index < vectorSize)
           {
               normals[index] = (normals[index] + pair.second.meanNormal).normalize();
           }
       }

   }

   m_vertices.resize(vectorSize);

   //Collect Vertices
   for (const auto& it : ht)
   {
       size_t index = it.second;
       if (index < vectorSize)
       {
         m_vertices[index] = { it.first.positions, normals.at(index)};
       }
   }
}

void BinaryFileDataAccessAsync::LoadTerrain_withSoftEdges(const wchar_t* filename)
{
    this->ReadFileSoftEdges(filename);
}

void BinaryFileDataAccessAsync::LoadTerrain_withSharpEdges(const wchar_t* filename)
{
    this->ReadFileSharpEdges(filename);
}

const std::vector<StlFacet>& BinaryFileDataAccessAsync::GetFacets(void)
{
    return m_facets;
}
bool BinaryFileDataAccessAsync::CreateCameraPose(CameraPose& cameraPose, const std::string& line, const std::vector<std::string>& headers)
{

    std::istringstream lineStream(line);
    std::string word;

    int i = -1;
    while ((std::getline(lineStream, word, ';'))) {
        i++;
        if (headers.at(i) == "sec") {
            cameraPose.epochtime.setSeconds(std::stoll(word));
        }
        else if (headers.at(i) == "nsec") {
            cameraPose.epochtime.setNanoseconds(std::stoll(word));
        }
        else if (headers.at(i) == "yaw") {
            cameraPose.yaw = std::stof(word);
        }
        else if (headers.at(i) == "pitch") {
            cameraPose.pitch = std::stof(word);
        }
        else if (headers.at(i) == "roll") {
            cameraPose.roll = std::stof(word);
        }
        else if (headers.at(i) == "north") {
            cameraPose.north = std::stof(word);
        }
        else if (headers.at(i) == "east") {
            cameraPose.east = std::stof(word);
        }
        else if (headers.at(i) == "down") {
            cameraPose.down = std::stof(word);
        }
    }

    return true;
}

bool BinaryFileDataAccessAsync::CheckTrajectoryHeaders(const std::vector<std::string>& headers)
{
    std::vector<std::string>::const_iterator it;
    std::vector<std::string> expectedHeaders = { "sec" , "nsec","yaw","pitch","roll","north","east","down" };

    for (std::vector<std::string>::const_iterator expected_it = expectedHeaders.cbegin(); expected_it != expectedHeaders.cend(); expected_it++)
    {
        std::vector<std::string>::const_iterator actual_it = std::find(headers.cbegin(), headers.cend(), *expected_it);
        if (actual_it == headers.cend())
        {
            return false;
        }

    }
    return true;
}

void BinaryFileDataAccessAsync::LoadCameraTrajectory(const wchar_t* filepath, std::vector<CameraPose>& cameraPoses)
{
    std::ifstream            file;
    std::string              line;
    std::string              word;
    std::vector<std::string> headers;
    CameraPose               cameraPose;

    file.open(filepath, std::ifstream::in);

    std::wstring errormsg = L"Failed to open " + std::wstring(filepath);
    THROW_TREXCEPTION_IF_FAILED(file.is_open(), errormsg);

    cameraPoses.clear();

    bool isCorrect = true;
    // read first line
    std::getline(file, line);

    //read the headers
    std::istringstream lineStream(line);
    while ((std::getline(lineStream, word, ';'))) {
        headers.push_back(word);
    }

    THROW_TREXCEPTION_IF_FAILED(CheckTrajectoryHeaders(headers), L"Incorrect header");
    while ((std::getline(file, line)) && isCorrect) {
        cameraPose = CameraPose();
        isCorrect = CreateCameraPose(cameraPose, line, headers);
        if (isCorrect) {
            cameraPoses.push_back(cameraPose);
        }
    }
    if (!isCorrect) {
        cameraPoses.clear();
    }

    file.close();

}


void BinaryFileDataAccessAsync::LoadConfigurationFile(const wchar_t* filepath, ParameterFile& params)
{
    nlohmann::json jsonFile;
    std::ifstream file(filepath);

    std::wstring errormsg = L"Failed to open " + std::wstring(filepath);
    THROW_TREXCEPTION_IF_FAILED(file.is_open(), errormsg);

    jsonFile = nlohmann::json::parse(file);
    params = jsonFile.get<ParameterFile>();
}