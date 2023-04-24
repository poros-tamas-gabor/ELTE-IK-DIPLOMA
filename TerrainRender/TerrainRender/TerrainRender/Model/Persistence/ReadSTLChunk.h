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

class ReadSTLChunkSharp : public ICallable
{
private:
	const std::wstring& m_filepath;
	int m_begin;
	int m_numOfFacets;
	//std::mutex& m_mutex;
	std::vector<stlFacet>* m_facets;
	

public:
	ReadSTLChunkSharp(const std::wstring& filepath, int beginInBytes, int numOfFacets, std::vector<stlFacet>* facets);
	void ReadChunk();
	virtual void operator()() override;
};

typedef std::shared_ptr<ReadSTLChunkSharp> ReadSTLChunkSharpPtr;


class ReadSTLChunkSoft : public ICallable
{
private:
	const std::wstring& m_filepath;
	int m_begin;
	unsigned m_numOfFacets;
	IndicesVecPtr m_indices;
	HashTable_Soft& m_ht;
	std::shared_mutex& m_mutex_hashtable;
	size_t& m_nextID;
	Map_Ind_NormalsPtr m_map_normals;

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