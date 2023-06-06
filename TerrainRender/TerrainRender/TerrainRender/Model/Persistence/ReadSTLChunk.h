#ifndef READ_STL_CHUNK_H
#define READ_STL_CHUNK_H

#include "ICallable.h"
#include <string>
#include "ModelStructs.h"
#include <mutex>
#include <shared_mutex>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
#include "DataAccess.h"

///////////////////////////////////////////////////////////////////////////////
// ReadSTLChunk.h
// ==============
//
// The classes ReadSTLChunkSharp and ReadSTLChunkSoft are used to read a chunk of data from a binary STL file. 
// Both classes inherit from the ICallable interface, which is used for multithreading purposes.
//
// ReadSTLChunkSharp reads the chunk of data and stores it in a vector of vertices. It is used by the LoadTerrain_withSharpEdges function.
//
// ReadSTLChunkSoft reads the chunk of data and stores it in a vector of indices and a hashtable of vertices. It is used by the LoadTerrain_withSoftEdges function.
// 
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

class ReadSTLChunkSharp : public ICallable
{
private:
	const std::wstring&		m_filepath;
	unsigned				m_begin;
	unsigned				m_numOfFacets;
	std::vector<StlFacet>&	m_facets;
	

public:
	ReadSTLChunkSharp(const std::wstring& filepath, int beginInBytes, int numOfFacets, std::vector<StlFacet>& facets);
	void ReadChunk();
	virtual void operator()() override;
};

typedef std::shared_ptr<ReadSTLChunkSharp> ReadSTLChunkSharpPtr;


class ReadSTLChunkSoft : public ICallable
{
private:
	const std::wstring&		m_filepath;
	unsigned				m_begin;
	unsigned				m_numOfFacets;
	IndicesVecPtr			m_indices;
	HashTable_Soft&			m_ht;
	std::shared_mutex&		m_mutex_hashtable;
	size_t&					m_nextID;
	Map_Ind_NormalsPtr		m_map_normals;

public:
	ReadSTLChunkSoft(const std::wstring& filepath, int beginInBytes, int numOfFacets, 
		IndicesVecPtr indices,
		HashTable_Soft& ht,
		std::shared_mutex& mutex_hashtable,
		size_t& nextID,
		Map_Ind_NormalsPtr m_map_normals
		);
	void ReadChunk();
	virtual void operator()() override;
};

typedef std::shared_ptr<ReadSTLChunkSoft> ReadSTLChunkSoftPtr;


#endif