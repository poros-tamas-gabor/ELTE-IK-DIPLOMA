#include "ReadSTLChunk.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../../ErrorHandler.h"
ReadSTLChunk::ReadSTLChunk(const std::wstring& filepath, int begin, int numOfFacets, std::vector<Facet>* facets) :
    m_filepath(filepath), m_begin(begin), m_numOfFacets(numOfFacets), m_facets(facets) {}


void ReadSTLChunk::operator()()
{
    ReadChunk();
}
void ReadSTLChunk::ReadChunk()
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
            Facet f;
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

