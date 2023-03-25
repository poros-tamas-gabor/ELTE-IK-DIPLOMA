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
int TextFileDataAccessAsync::GetNumThreads(int fileSize)
{
    int maxNumThreads = std::thread::hardware_concurrency();
    int numThreads;
    
    numThreads = fileSize / minChunkSize;
    numThreads = max(1, numThreads);
    numThreads = min(maxNumThreads, numThreads);

    return numThreads;
}

bool TextFileDataAccessAsync::ReadFile(const std::wstring& filepath, ICallableCreatorPtr creator)
{
    // Get filesize
    std::ifstream file(filepath, std::ios::ate); // Open file and seek to end
    if (!file.is_open())
        return false;

    int fileSize = file.tellg();

    int numThreads = GetNumThreads(fileSize);

    //get the chunkSize;
    int chunkSize = fileSize / numThreads;

    std::vector<std::thread> threads;
    std::vector<ICallablePtr> processes;

    for (int i = 0; i < numThreads; i++)
    {
        int start = i * chunkSize;
        int end = (i == numThreads - 1) ? fileSize : (i + 1) * chunkSize;
        creator->Initialize(filepath, start, end, this);
        processes.push_back(creator->Create());
        threads.emplace_back(std::thread(std::ref(*processes.at(i))));
    }

    for (int i = 0; i < numThreads; i++)
    {
        if (threads.at(i).joinable())
            threads.at(i).join();
    }

    // Clean up and exit
    file.close();
    return true;
}




bool TextFileDataAccessAsync::LoadTerrain(const wchar_t* filename, std::vector<VertexMesh>& vertices)
{
    std::time_t now = std::time(NULL);
    m_faces.clear();
    ICallableCreatorPtr creator = std::make_shared<ReadSTLChunkCreator>();
    vertices.clear();

    try
    {

    this->ReadFile(filename, creator);

    for (const Facet& facet : m_faces)
    {
        for (int i = 0; i < 3; i++)
        {
            VertexMesh vertex;
            vertex.color = { 1.0f, 1.0f, 1.0f, 1.0f };
            vertex.normal = { (float)facet.normal.x, (float)facet.normal.y,(float)facet.normal.z };
            vertex.position = { (float)facet.position[i].x, (float)facet.position[i].y, (float)facet.position[i].z };

            vertices.push_back(vertex);
        }
    }
    std::time_t end = std::time(NULL);
    std::wstring str = L"Loading time : in sec: ";
    str += std::to_wstring(end - now);
    str += L"\n";
    OutputDebugString(str.c_str());
    return !vertices.empty();
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
bool TextFileDataAccessAsync::LoadCameraTrajectory(const wchar_t* filename, std::vector<CameraPose>& cameraPoses)
{
    TextFileDataAccess p;
    return p.LoadCameraTrajectory(filename, cameraPoses);
}
