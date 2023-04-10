#ifndef READ_STL_CHUNK_H
#define READ_STL_CHUNK_H

#include "ICallable.h"
#include <string>
#include "ModelStructs.h"
#include <mutex>
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
	int m_numOfFacets;
	std::vector<StlVertex>& m_vertices;
	std::vector<FacetCornerIndices>& m_indices;
	std::unordered_map<VertexHTindex, NormalsInSamePositions, VertexHTindex::Hash>& m_ht;
	std::mutex& m_mutex_vertices;
	std::mutex& m_mutex_indices;
	std::mutex& m_mutex_hashtable;

public:
	ReadSTLChunkSoft(const std::wstring& filepath, int beginInBytes, int numOfFacets, 
		std::vector<StlVertex>& vertices,
		std::vector<FacetCornerIndices>& indices,
		std::unordered_map<VertexHTindex, NormalsInSamePositions, VertexHTindex::Hash>& ht,
		std::mutex& mutex_vertices,
		std::mutex& mutex_indices,
		std::mutex& mutex_hashtable
		);
	void ReadChunk();
	virtual void operator()() override;
};

typedef std::shared_ptr<ReadSTLChunkSoft> ReadSTLChunkSoftPtr;


#endif