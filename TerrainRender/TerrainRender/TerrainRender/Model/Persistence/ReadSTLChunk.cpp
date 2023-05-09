#include "ReadSTLChunk.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../../ErrorHandler.h"
ReadSTLChunkSharp::ReadSTLChunkSharp(const std::wstring& filepath, int begin, int numOfFacets, std::vector<StlFacet>& facets) :
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
            StlFacet f;
            file.read(reinterpret_cast<char*>(f.normal), 12);
            for (unsigned j = 0; j < 3; j++)
            {
                file.read(reinterpret_cast<char*>(f.position[j]), 12);
            }
            file.seekg(2, std::ios_base::cur);
            m_facets.push_back(f);
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


Vector3D GetNormal(const std::array<Vector3D, 3>& vertices, unsigned index)
{
    unsigned i = (index + 1) % 3;
    unsigned ip1 = (index + 2) % 3;

    Vector3D a_i = vertices.at(i) - vertices.at(index);
    Vector3D a_ip1 = vertices.at(ip1) - vertices.at(index);

    Vector3D product = a_i.crossProduct(a_ip1);

    float factor = a_i.squareLength() * a_ip1.squareLength();

    Vector3D normal = product * factor;

    return normal.normalize();

}
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
            CornerIndices           facet;
            std::array<Vector3D, 3> vertices;

            file.seekg(12, std::ios_base::cur);

            for (unsigned j = 0; j < 3; j++)
            {
                float c[3];
                file.read(reinterpret_cast<char*>(c), 12);
                vertices.at(j) = { c[0], c[1], c[2] };
            }

            for (unsigned j = 0; j < 3; j++)
            {
                Vector3D normal = GetNormal(vertices, j);
                HTindex_Soft vertexHashIndex = { to_float_with_precision(vertices.at(j).x), to_float_with_precision(vertices.at(j).y), to_float_with_precision(vertices.at(j).z) };
              
                //find in hash table vertexPosstr
                std::shared_lock<std::shared_mutex> shared_lock_ht(m_mutex_hashtable);
                auto it = m_ht.find(vertexHashIndex);
                //if found
                if (it != m_ht.end())
                {
                    size_t vertexIndex = it->second;
                    shared_lock_ht.unlock();

                    facet.corner[j] = vertexIndex;
                    
                    if (m_map_normals->find(vertexIndex) != m_map_normals->end())
                    {
                        m_map_normals->at(vertexIndex).normals.push_back(normal);
                    }
                }
                else
                {
                    shared_lock_ht.unlock();
                    std::unique_lock<std::shared_mutex> unique_lock_ht(m_mutex_hashtable);

                    it = m_ht.find(vertexHashIndex);
                    if (it != m_ht.end())
                    {
                        size_t vertexIndex = it->second;
                        unique_lock_ht.unlock();
                        facet.corner[j] = vertexIndex;

                        if (m_map_normals->find(vertexIndex) != m_map_normals->end())
                        {
                            m_map_normals->at(vertexIndex).normals.push_back(normal);
                        }
                    }
                    else
                    {
                        size_t vertexIndex = m_nextID++;
                        m_ht.insert(std::pair<HTindex_Soft, size_t>(vertexHashIndex, vertexIndex));

                        unique_lock_ht.unlock();


                        facet.corner[j] = vertexIndex;

                        NormalsInSamePositions vn;
                        vn.normals.push_back(normal);

                        m_map_normals->insert(std::pair<size_t, NormalsInSamePositions>(vertexIndex, vn));
                    }
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
