#include "ReadSTLChunk.h"

ReadSTLChunk::ReadSTLChunk(const std::wstring& filepath, int start, int end, TextFileDataAccessAsync* persistence) : 
    m_filepath(filepath), m_start(start), m_end(end), m_mutex(persistence->m_mutex), m_faces(persistence->m_faces) {}
void ReadSTLChunk::ReadChunk()
{
    std::ifstream file(m_filepath);

    if (!file.is_open())
        return;

    file.seekg(m_start, std::ios_base::beg);

    std::string line;
    STLLineType type = BEGIN;

    if (m_start == 0) {
        std::getline(file, line); // skip header
        type = FACET;
    }
    else if (std::getline(file, line) && line.find("endfacet") != std::string::npos) {
        type = FACET;
    }
    else {
        int step_back = 0;
        while (std::getline(file, line) && line.find("endfacet") == std::string::npos && step_back < m_start) {
            file.seekg(m_start - step_back, std::ios_base::beg);
            step_back++;
        }
        type = FACET;
    }

    int pos = file.tellg();
    Facet facet;
    int vertexCount = 0;
    while (pos < m_end && std::getline(file, line)) {
        CreateFacet(facet, line, type, vertexCount);
        pos = file.tellg();
    }

    file.close();
}
bool ReadSTLChunk::CreateFacet(Facet& facet, const std::string& line, STLLineType& type, int& vertexCount)
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
            std::unique_lock<std::mutex> lock(m_mutex);
            m_faces.push_back(facet);
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
void ReadSTLChunk::operator()()
{
    ReadChunk();
}

void ReadSTLChunkCreator::Initialize(const std::wstring& filepath, int start, int end, TextFileDataAccessAsync* persistence)
{
    this->filepath = filepath;
    this->start = start;
    this->end = end;
    this->persistence = persistence;
}
ICallablePtr ReadSTLChunkCreator::Create()
{
    return std::make_shared<ReadSTLChunk>(filepath, start, end, persistence);
}