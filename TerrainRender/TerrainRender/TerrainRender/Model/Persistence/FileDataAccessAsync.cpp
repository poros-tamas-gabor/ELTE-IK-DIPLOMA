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
        std::vector<std::vector<Facet>> facetVectors(numThreads);

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


bool BinaryFileDataAccessAsync::LoadTerrain(const wchar_t* filename)
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

const std::vector<Facet>& BinaryFileDataAccessAsync::GetFacets(void)
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

bool BinaryFileDataAccessAsync::LoadCameraTrajectory(const wchar_t* filename, std::vector<CameraPose>& cameraPoses)
{
    std::ifstream            input;
    std::string              line;
    std::string              word;
    std::vector<std::string> headers;
    CameraPose               cameraPose;

    input.open(filename, std::ifstream::in);

    if (!input.is_open())
        return false;

    cameraPoses.clear();


    try
    {
        bool isCorrect = true;
        // read first line
        std::getline(input, line);

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

        while ((std::getline(input, line)) && isCorrect) {
            cameraPose = CameraPose();
            isCorrect = CreateCameraPose(cameraPose, line, headers);
            if (isCorrect) {
                cameraPoses.push_back(cameraPose);
            }
        }
        if (!isCorrect) {
            cameraPoses.clear();
        }

        input.close();

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