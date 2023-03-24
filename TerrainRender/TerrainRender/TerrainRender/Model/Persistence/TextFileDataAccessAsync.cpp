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

void TextFileDataAccessAsync::ReadChunk(const std::wstring& filepath, int start, int end)
{
    std::ifstream file(filepath);

    if (!file.is_open())
        return;

    file.seekg(start, std::ios_base::beg);

    std::string line;
    STLLineType type = BEGIN;

    if (start == 0) {
        std::getline(file, line); // skip header
        type = FACET;
    }
    else if (std::getline(file, line) && line.find("endfacet") != std::string::npos) {
        type = FACET;
    }
    else {
        int step_back = 0;
        while (std::getline(file, line) && line.find("endfacet") == std::string::npos && step_back < start) {
            file.seekg(start - step_back, std::ios_base::beg);
            step_back++;
        }
        type = FACET;
    }

    int pos = file.tellg();
    Facet facet;
    int vertexCount = 0;
    while (pos < end && std::getline(file, line)) {
        CreateFacet(facet, line, type, vertexCount);
        pos = file.tellg();
    }

    file.close();
}

int TextFileDataAccessAsync::GetNumThreads(int fileSize)
{
    int maxNumThreads = std::thread::hardware_concurrency();
    int numThreads;
    
    numThreads = fileSize / minChunkSize;
    numThreads = max(1, numThreads);
    numThreads = min(maxNumThreads, numThreads);

    return numThreads;
}

bool TextFileDataAccessAsync::ReadFile(const std::wstring& filepath)
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

    for (int i = 0; i < numThreads; i++)
    {
        int start = i * chunkSize;
        int end = (i == numThreads - 1) ? fileSize : (i + 1) * chunkSize;
        std::thread t(&TextFileDataAccessAsync::ReadChunk,this, filepath, start, end);
        threads.emplace_back(std::move(t));
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
bool TextFileDataAccessAsync::CreateFacet(Facet& facet, const std::string& line, STLLineType& type, int& vertexCount)
{
    bool bresult;
    switch (type) {
    case BEGIN: {
        type = FACET;
        return true;
    }
    case FACET:
    case END: {
        facet.normal = { 0,0,0 };
        facet.position[0] = facet.position[1] = facet.position[2] = { 0,0,0 };

        if (line.find("endsolid") != std::string::npos) {
            return true;
        }

        std::istringstream iss(line);
        std::string word;
        iss >> word >> word; // skip "facet" and "normal"
        iss >> facet.normal.x >> facet.normal.y >> facet.normal.z;
        type = LOOP;
        return true;
    }
    case LOOP: {
        type = VERTEX;
        return true;
    }
    case VERTEX: {
        std::istringstream iss(line);
        std::string word;
        iss >> word; // skip "vertex"
        iss >> facet.position[vertexCount].x >> facet.position[vertexCount].y >> facet.position[vertexCount].z;



        if (vertexCount < 2) {
            vertexCount++;
            type = VERTEX;
        }
        else {
            vertexCount = 0;
            type = ENDLOOP;
            // Wait for a facet to be added to the queue
            std::unique_lock<std::mutex> lock(queueMutex);
            faces.push_back(facet);
            lock.unlock();
        }
        return true;
    }
    case ENDLOOP: {
        type = ENDFACET;
        return true;
    }
    case ENDFACET: {
        type = FACET;
        return true;
    }
    }
}



bool TextFileDataAccessAsync::LoadTerrain(const wchar_t* filename, std::vector<Vertex>& vertices)
{
    std::time_t now = std::time(NULL);
    faces.clear();
    this->ReadFile(filename);

    vertices.clear();

    for (Facet facet : faces)
    {
        for (int i = 0; i < 3; i++)
        {
            Vertex vertex;
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
    return true;
}
bool TextFileDataAccessAsync::LoadCameraTrajectory(const wchar_t* filename, std::vector<CameraPose>& cameraPoses)
{
    return false;
}