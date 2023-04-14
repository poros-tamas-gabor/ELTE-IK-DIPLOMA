#include "ReadSTLChunk.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../../ErrorHandler.h"
ReadSTLChunkSharp::ReadSTLChunkSharp(const std::wstring& filepath, int begin, int numOfFacets, std::vector<stlFacet>* facets) :
    m_filepath(filepath), m_begin(begin), m_numOfFacets(numOfFacets), m_facets(facets) {}


void ReadSTLChunkSharp::operator()()
{
    ReadChunk();
}
void ReadSTLChunkSharp::ReadChunk()
{
    try
    {
        std::ifstream file(m_filepath, std::ios::binary);
        
        std::wstring errormsg = L"Failed to open " + std::wstring(m_filepath);
        THROW_TREXCEPTION_IF_FAILED(file.is_open(), errormsg);
        
        file.seekg(m_begin, std::ios::beg);
        // Read the file data into the buffer
        for (unsigned i = 0; i < m_numOfFacets; i++)
        {
            stlFacet f;
            file.read(reinterpret_cast<char*>(f.normal), 12);
            for (unsigned j = 0; j < 3; j++)
            {
                file.read(reinterpret_cast<char*>(f.position[j]), 12);
            }
            file.seekg(2, std::ios_base::cur);
            m_facets->push_back(f);
        }
        // Clean up
        file.close();
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

}


ReadSTLChunkSoft::ReadSTLChunkSoft(const std::wstring& filepath, int beginInBytes, int numOfFacets,
    IndicesVecPtr indices,
    HashTable_Soft& ht,
    std::mutex& mutex_hashtable,
    size_t& nextID) 
    :
    m_filepath(filepath), m_begin(beginInBytes), m_numOfFacets(numOfFacets),
    m_indices(indices), m_ht(ht), m_mutex_hashtable(mutex_hashtable), m_nextID(nextID) {}
void ReadSTLChunkSoft::ReadChunk()
{
    try
    {
        std::ifstream file(m_filepath, std::ios::binary);

        std::wstring errormsg = L"Failed to open " + std::wstring(m_filepath);
        THROW_TREXCEPTION_IF_FAILED(file.is_open(), errormsg);

        file.seekg(m_begin, std::ios::beg);
        // Read the file data into the buffer
        for (unsigned i = 0; i < m_numOfFacets; i++)
        {
            float n[3];
            CornerIndices facet;
            file.read(reinterpret_cast<char*>(n), 12);
            Vector3D normal = { n[0], n[1], n[2] };
            normal.normalize();
            
            for (unsigned j = 0; j < 3; j++)
            {
                float c[3];
                file.read(reinterpret_cast<char*>(c), 12);
                HTindex_Soft vertexHashIndex = { to_float_with_precision(c[0]), to_float_with_precision(c[1]), to_float_with_precision(c[2]) };
             
            
                //find in hash table vertexPosstr
                std::unique_lock<std::mutex> lock_hashtable(m_mutex_hashtable);
                auto it = m_ht.find(vertexHashIndex);
                //if found
                if (it != m_ht.end())
                {
                    it->second.normals.push_back(normal);
                    facet.corner[j] = it->second.vertIndex;
                }
                else
                {
                    size_t vertexIndex = m_nextID++;
                    NormalsInSamePositions vn;
                    vn.vertIndex = vertexIndex;
                    vn.normals.clear();
                    vn.normals.push_back(normal);
                    m_ht.insert(std::pair<HTindex_Soft, NormalsInSamePositions>(vertexHashIndex, vn));

                    facet.corner[j] = vertexIndex;
                }
                lock_hashtable.unlock();
            }
            m_indices->push_back(facet);
            file.seekg(2, std::ios_base::cur);
        }
        // Clean up
        file.close();
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
}
void ReadSTLChunkSoft::operator()()
{
    ReadChunk();
}
