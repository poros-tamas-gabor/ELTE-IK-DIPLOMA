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

class ReadSTLChunk : public ICallable
{
private:
	const std::wstring& m_filepath;
	int m_begin;
	int m_numOfFacets;
	//std::mutex& m_mutex;
	std::vector<stlFacet>* m_facets;
	

public:
	ReadSTLChunk(const std::wstring& filepath, int beginInBytes, int numOfFacets, std::vector<stlFacet>* facets);
	void ReadChunk();
	virtual void operator()() override;
};

typedef std::shared_ptr<ReadSTLChunk> ReadSTLChunkPtr;

#endif