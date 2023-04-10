#include "DataAccess.h"
#include "DataAccess.h"

#include <fstream>
#include <string>
#include <windows.h>
#include "ModelStructs.h"
#include <vector>
#include <thread>
#include <sstream>
#include <mutex>

#include <memory>
#include "../../ErrorHandler.h"
#include "../../nlohmann/json.hpp"
int BinaryFileDataAccessAsync::GetNumThreads(int numOfFacets)
{
    int maxNumThreads = std::thread::hardware_concurrency();
    int numThreads;
    
    numThreads = numOfFacets / minChunkSize;
    numThreads = max(1, numThreads);
    numThreads = min(maxNumThreads, numThreads);

    return numThreads;
}

bool BinaryFileDataAccessAsync::ReadFile(const std::wstring& filepath)
{
    try
    {
        std::ifstream file(filepath, std::ios::binary); // Open file and seek to end
        std::wstring errormsg = L"Failed to open " + std::wstring(filepath);
        THROW_TREXCEPTION_IF_FAILED(file.is_open(), errormsg);

        m_faces.clear();

        // Get the size of the file
        file.seekg(0, std::ios::end);
        int fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        // Read the number of triangles
        int num_triangles;
        file.seekg(80, std::ios_base::beg);
        file.read(reinterpret_cast<char*>(&num_triangles), sizeof(num_triangles));
        errormsg = L"Invalid binary STL file" + std::wstring(filepath);
        THROW_TREXCEPTION_IF_FAILED((fileSize == 84 + 50 * num_triangles), errormsg);

        file.close();

        // Get the number of facets in the file
        unsigned numOfFacets = (fileSize - 84) / 50;

        // Calculate how many threads are needed
        int numThreads = GetNumThreads(numOfFacets);
        // Calculate how many facets are in a chunk
        int numOfFacetInChunk = numOfFacets / numThreads;

        std::vector<std::thread> threads;
        std::vector<ICallablePtr> processes;
        std::vector<std::vector<stlFacet>> facetVectors(numThreads);

        for (int i = 0; i < numThreads; i++)
        {
            unsigned currentNumOfFacets;
            unsigned beginInBytes = 84 + 50 * numOfFacetInChunk * i;
            if (i < numThreads - 1)
                currentNumOfFacets = numOfFacetInChunk;
            else
                currentNumOfFacets = numOfFacets - i * numOfFacetInChunk;

            processes.emplace_back(std::make_shared<ReadSTLChunk>(filepath, beginInBytes, currentNumOfFacets, &facetVectors.at(i)));
            threads.emplace_back(std::thread(std::ref(*processes.at(i))));
        }

        for (int i = 0; i < numThreads; i++)
        {
            if (threads.at(i).joinable())
                threads.at(i).join();
        }
        for (int i = 0; i < numThreads; i++)
        {
            m_faces.insert(m_faces.end(), facetVectors.at(i).begin(), facetVectors.at(i).end());
        }
        return true;
    }
    catch (TRException& e)
    {
        ErrorHandler::Log(e);
    }
    catch (std::exception& e)
    {
        ErrorHandler::Log(e);
    }
    return false;
}
std::string to_string_with_precision(const float a_value, const int n = 6)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return std::move(out).str();
}

const std::vector<StlVertex>& BinaryFileDataAccessAsync::GetSolidVertices()
{
    return this->m_vertices;
}
const std::vector<FacetCornerIndices>& BinaryFileDataAccessAsync::GetSolidIndices()
{
    return this->m_facets;
}
bool BinaryFileDataAccessAsync::ReadFileSolid(const std::wstring& filepath)
{
    try
    {
        std::ifstream file(filepath, std::ios::binary); // Open file and seek to end
        std::wstring errormsg = L"Failed to open " + std::wstring(filepath);
        THROW_TREXCEPTION_IF_FAILED(file.is_open(), errormsg);


        // Get the size of the file
        file.seekg(0, std::ios::end);
        int fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        // Read the number of triangles
        int num_triangles;
        file.seekg(80, std::ios_base::beg);
        file.read(reinterpret_cast<char*>(&num_triangles), sizeof(num_triangles));
        errormsg = L"Invalid binary STL file" + std::wstring(filepath);
        THROW_TREXCEPTION_IF_FAILED((fileSize == 84 + 50 * num_triangles), errormsg);


        // Get the number of facets in the file
        unsigned numOfFacets = (fileSize - 84) / 50;

        errormsg = L"Failed to open " + std::wstring(filepath);
        THROW_TREXCEPTION_IF_FAILED(file.is_open(), errormsg);


        std::unordered_map<VertexHTindex, NormalsInSamePositions, VertexHTindex::Hash> ht;
        m_vertices.clear();
        m_facets.clear();

        // Read the file data into the buffer
        for (unsigned i = 0; i < numOfFacets; i++)
        {
            float n[3];
            FacetCornerIndices facet;
            file.read(reinterpret_cast<char*>(n), 12);
            Vector3D normal = { n[0], n[1], n[2] };
            normal.normalize();

            for (unsigned j = 0; j < 3; j++)
            {
                float c[3];
                file.read(reinterpret_cast<char*>(c), 12);
                VertexHTindex vertexHashIndex = { to_string_with_precision(c[0]), to_string_with_precision(c[1]), to_string_with_precision(c[2]) };

                //find in hash table vertexPosstr
               auto it = ht.find(vertexHashIndex);
               //if found
               if (it != ht.end())
               {
                   it->second.normals.push_back(normal);
                   facet.corner[j] = it->second.vertIndex;
               }
               else
               {
                    StlVertex vertex;
                    vertex.pos = { c[0], c[1], c[2] };
                    m_vertices.push_back(vertex);
                    //last pushed element index
                    facet.corner[j] = m_vertices.size() - 1;

                    NormalsInSamePositions vn;
                    vn.vertIndex = m_vertices.size() - 1;
                    vn.normals.clear();
                    vn.normals.push_back(normal);
                    ht.insert(std::pair<VertexHTindex, NormalsInSamePositions>(vertexHashIndex, vn));
                }
            }
            m_facets.push_back(facet);
            file.seekg(2, std::ios_base::cur);

        }
        // Clean up
        file.close();

        int i = 0;
        for (StlVertex& v : m_vertices)
        {
            VertexHTindex vertexHashIndex = { to_string_with_precision(v.pos.x), to_string_with_precision(v.pos.y), to_string_with_precision(v.pos.z) };
            //find in hash table vertexPosstr
            auto it = ht.find(vertexHashIndex);
            if (it != ht.end())
            {
                v.normal = it->second.sumNormals();
            }
            i++;
        }


        return true;
    }
    catch (TRException& e)
    {
        ErrorHandler::Log(e);
    }
    catch (std::exception& e)
    {
        ErrorHandler::Log(e);
    }
    return false;
}

bool BinaryFileDataAccessAsync::LoadTerrainSoftEdges(const wchar_t* filename)
{
    std::time_t now = std::time(NULL);
    bool success = false;

    try
    {

        success = this->ReadFileSolid(filename);

        std::time_t end = std::time(NULL);
        std::wstring str = L"Loading time : in sec: ";
        str += std::to_wstring(end - now);
        str += L"\n";
        OutputDebugString(str.c_str());
        return success;
    }

    catch (TRException& e)
    {
        ErrorHandler::Log(e);
    }
    catch (std::exception& e)
    {
        ErrorHandler::Log(e);
    }
    catch (...)
    {
        ErrorHandler::Log("Unknown exception");
    }
    return false;
}

bool BinaryFileDataAccessAsync::LoadTerrainSharpEdges(const wchar_t* filename)
{
    std::time_t now = std::time(NULL);
    m_faces.clear();
    bool success = false;


    try
    {

     success = this->ReadFile(filename);

    std::time_t end = std::time(NULL);
    std::wstring str = L"Loading time : in sec: ";
    str += std::to_wstring(end - now);
    str += L"\n";
    OutputDebugString(str.c_str());
    return success;
    }

    catch (TRException& e)
    {
        ErrorHandler::Log(e);
    }
    catch (std::exception& e)
    {
        ErrorHandler::Log(e);
    }
    catch (...)
    {
        ErrorHandler::Log("Unknown exception");
    }
    return false;
}

const std::vector<stlFacet>& BinaryFileDataAccessAsync::GetFacets(void)
{
    return m_faces;
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

bool BinaryFileDataAccessAsync::LoadCameraTrajectory(const wchar_t* filepath, std::vector<CameraPose>& cameraPoses)
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


    try
    {
        bool isCorrect = true;
        // read first line
        std::getline(file, line);

        //read the headers
        std::istringstream lineStream(line);
        while ((std::getline(lineStream, word, ';'))) {
            headers.push_back(word);
        }

        std::vector<std::string>::iterator it;
        //TODO: CHECK headers
        for (it = headers.begin(); it != headers.end(); it++)
        {
            if (*it == "nsec")
                break;
        }
        if (it == headers.end())
        {
            THROW_TREXCEPTION(L"nsec missing");
        }

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

        return isCorrect;
    }
    catch (TRException& e)
    {
        ErrorHandler::Log(e);
    }
    catch (std::exception& e)
    {
        ErrorHandler::Log(e);
    }
    catch (...)
    {
        ErrorHandler::Log("Unknown exception");
    }
    return false;

}

void from_json(const nlohmann::json& json, LLACoordinate& data)
{
    data.latitude = json.at("latitude").get<double>();
    data.longitude = json.at("longitude").get<double>();
}

Vector3D to_Vector3D(const std::vector<float>& stdvec)
{
    Vector3D vec;
    THROW_TREXCEPTION_IF_FAILED( (stdvec.size() == 3) , L"Failed to load Vector3D");
    vec.x = stdvec.at(0);
    vec.y = stdvec.at(1);
    vec.z = stdvec.at(2);
    return vec;
}

Vector4D to_Vector4D(const std::vector<float>& stdvec)
{
    Vector4D vec;
    THROW_TREXCEPTION_IF_FAILED( (stdvec.size() == 4) , L"Failed to load Vector4D");
    vec.x = stdvec.at(0);
    vec.y = stdvec.at(1);
    vec.z = stdvec.at(2);
    vec.w = stdvec.at(3);
    return vec;
}

void from_json(const nlohmann::json& json, ParameterFile::Terrain& data)
{
    std::vector<float> tmp = json.at("translation").get<std::vector<float>>();
    data.translation = to_Vector3D(tmp);
    tmp = json.at("rotation").get<std::vector<float>>();
    data.rotation = to_Vector3D(tmp);
    
    for (auto it = json.at("color").begin(); it != json.at("color").end(); ++it) {
        std::string color_name = it.key();
        std::vector<float> color_values = it.value();
        Vector4D vec = to_Vector4D(color_values);
        data.colors.insert(std::pair<std::string, Vector4D>(color_name, vec));
    }
}

void from_json(const nlohmann::json& json, ParameterFile::Trajectory& data)
{
    std::vector<float> tmp = json.at("translation").get<std::vector<float>>();
    data.translation = to_Vector3D(tmp);
    tmp = json.at("rotation").get<std::vector<float>>();
    data.rotation = to_Vector3D(tmp);
}

void from_json(const nlohmann::json& j, ParameterFile& p) {
    p.origo = j.at("origo").get<LLACoordinate>();
    p.terrain = j.at("terrain").get<ParameterFile::Terrain>();
    p.trajectory = j.at("trajectory").get<ParameterFile::Trajectory>();
}

bool BinaryFileDataAccessAsync::LoadParameterFile(const wchar_t* filepath, ParameterFile& params)
{
    try
    {
        nlohmann::json jsonFile;
        std::ifstream file(filepath);

        std::wstring errormsg = L"Failed to open " + std::wstring(filepath);
        THROW_TREXCEPTION_IF_FAILED(file.is_open(), errormsg);

        jsonFile = nlohmann::json::parse(file);
        params = jsonFile.get<ParameterFile>();

        return true;
    }
    catch (nlohmann::json::parse_error& e)
    {
        ErrorHandler::Log(e.what());
    }
    catch (TRException& e)
    {
        ErrorHandler::Log(e);
    }
    catch (std::exception& e)
    {
        ErrorHandler::Log(e);
    }
    catch (...)
    {
        ErrorHandler::Log("Unknown exception");
    }
    return false;

}