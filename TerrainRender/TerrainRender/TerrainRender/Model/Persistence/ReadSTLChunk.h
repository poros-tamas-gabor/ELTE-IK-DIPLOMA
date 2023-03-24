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
	int m_start;
	int m_end;
	std::mutex& m_mutex;
	std::vector<Facet>& m_faces;
	

public:
	ReadSTLChunk(const std::wstring& filepath, int start, int end, TextFileDataAccessAsync* persistence);
	void ReadChunk();
	bool CreateFacet(Facet& facet, const std::string& line, STLLineType& type, int& vertexCount);
	virtual void operator()() override;
};

typedef std::shared_ptr<ReadSTLChunk> ReadSTLChunkPtr;

class ReadSTLChunkCreator : public ICallableCreator
{
private:
	std::wstring filepath;
	int start;
	int end;
	TextFileDataAccessAsync* persistence;

public:
	virtual void Initialize(const std::wstring& filepath, int start, int end, TextFileDataAccessAsync* persistence) override;
	virtual ICallablePtr Create() override;
};

typedef std::shared_ptr<ReadSTLChunkCreator> ReadSTLChunkCreatorPtr;
#endif