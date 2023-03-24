#ifndef PERSISTENCE_H
#define	PERSISTENCE_H
#include "ModelStructs.h"
#include "../Vertex.h"
#include <vector>
#include <sstream>
#include <mutex>


class IDataAccess
{
public:
	virtual ~IDataAccess() = default;
	virtual bool LoadTerrain(const wchar_t*, std::vector<Vertex>&) = 0;
    virtual bool LoadCameraTrajectory(const wchar_t*, std::vector<CameraPose>&) = 0;
};

class TextFileDataAccess : public IDataAccess {

private:
    enum STLLineType { BEGIN, FACET, LOOP, VERTEX, ENDLOOP, ENDFACET, END };

private:
    bool CreateVertex(Vertex& vertex, const std::string& line, std::vector<Vertex>& vertices, STLLineType& type, int& vertexCount);
    bool CreateCameraPose(CameraPose& cameraPose, const std::string& line, const std::vector<std::string>& headers);

public:
    TextFileDataAccess() = default;
    TextFileDataAccess(const TextFileDataAccess&) = delete;
    TextFileDataAccess& operator=(const TextFileDataAccess&) = delete;
    ~TextFileDataAccess() = default;
    bool LoadTerrain(const wchar_t* filename, std::vector<Vertex>& vertices) override;
    bool LoadCameraTrajectory(const wchar_t* filename, std::vector<CameraPose>& cameraPoses) override;
};

class TextFileDataAccessAsync : public IDataAccess {



private:
    enum STLLineType { BEGIN, FACET, LOOP, VERTEX, ENDLOOP, ENDFACET, END };
private:

private:
    const int minChunkSize = 2629781;
    std::mutex queueMutex;
    std::vector<Facet> faces;
private:
    void ReadChunk(const std::wstring& filepath, int start, int end);
    bool ReadFile(const std::wstring& filepath);
    bool CreateFacet(Facet& facet, const std::string& line, STLLineType& type, int& vertexCount);
    int GetNumThreads(int fileSize);

public:
    TextFileDataAccessAsync() = default;
    TextFileDataAccessAsync(const TextFileDataAccessAsync&) = delete;
    TextFileDataAccessAsync& operator=(const TextFileDataAccessAsync&) = delete;
    ~TextFileDataAccessAsync() = default;
    bool LoadTerrain(const wchar_t* filename, std::vector<Vertex>& vertices) override;
    bool LoadCameraTrajectory(const wchar_t* filename, std::vector<CameraPose>& cameraPoses) override;
};
#endif // ! PERSISTENCE_H
