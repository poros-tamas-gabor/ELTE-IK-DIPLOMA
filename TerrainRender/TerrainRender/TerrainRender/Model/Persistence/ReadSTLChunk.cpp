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
    std::shared_mutex& mutex_hashtable,
    size_t& nextID,
    Map_Ind_NormalsPtr map_normals)
    :
    m_filepath(filepath), m_begin(beginInBytes), m_numOfFacets(numOfFacets),
    m_indices(indices), m_ht(ht), m_mutex_hashtable(mutex_hashtable), m_nextID(nextID), m_map_normals(map_normals) {}
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
                std::shared_lock<std::shared_mutex> lock_hashtable(m_mutex_hashtable);
                auto it = m_ht.find(vertexHashIndex);
                //if found
                if (it != m_ht.end())
                {
                    size_t vertexIndex = it->second;
                    lock_hashtable.unlock();

                    facet.corner[j] = vertexIndex;
                    
                    if (m_map_normals->find(vertexIndex) != m_map_normals->end())
                    {
                        m_map_normals->at(vertexIndex).normals.push_back(normal);
                    }
                }
                else
                {
                    lock_hashtable.unlock();
                    std::unique_lock<std::shared_mutex> lock_ht(m_mutex_hashtable);
                    size_t vertexIndex = m_nextID++;
                    m_ht.insert(std::pair<HTindex_Soft, size_t>(vertexHashIndex, vertexIndex));
                    lock_ht.unlock();

                    facet.corner[j] = vertexIndex;

                    NormalsInSamePositions vn;
                    vn.normals.push_back(normal);

                    m_map_normals->insert(std::pair<size_t, NormalsInSamePositions>(vertexIndex, vn));
                }
                
            }
            m_indices->push_back(facet);
            file.seekg(2, std::ios_base::cur);
        }
        // Clean up
        file.close();

        for (auto& pair : *m_map_normals.get())
        {
            pair.second.sumNormals();
        }
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
