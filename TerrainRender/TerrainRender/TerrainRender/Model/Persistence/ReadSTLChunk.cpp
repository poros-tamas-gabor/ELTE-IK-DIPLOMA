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
    std::vector<StlVertex>& vertices,
    std::vector<FacetCornerIndices>& indices,
    std::unordered_map<VertexHTindex, NormalsInSamePositions, VertexHTindex::Hash>& ht,
    std::mutex& mutex_vertices,
    std::mutex& mutex_indices,
    std::mutex& mutex_hashtable) :m_filepath(filepath), m_begin(beginInBytes), m_numOfFacets(numOfFacets),
    m_vertices(vertices), m_indices(indices), m_ht(ht), m_mutex_vertices(mutex_vertices), m_mutex_indices(mutex_indices), m_mutex_hashtable(mutex_hashtable) {}
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
                    StlVertex vertex;
                    vertex.pos = { c[0], c[1], c[2] };
                    size_t lastIndex;
                    m_vertices.push_back(vertex);
                    lastIndex = m_vertices.size() - 1;

                    //last pushed element index
                    facet.corner[j] = lastIndex;
            
                    NormalsInSamePositions vn;
                    vn.vertIndex = lastIndex;
                    vn.normals.clear();
                    vn.normals.push_back(normal);
                    m_ht.insert(std::pair<VertexHTindex, NormalsInSamePositions>(vertexHashIndex, vn));
                }
                lock_hashtable.unlock();
            }
            std::unique_lock<std::mutex> lock_indices(m_mutex_indices);
            m_indices.push_back(facet);
            lock_indices.unlock();
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
