#ifndef PERSISTENCE_H
#define	PERSISTENCE_H
#include "ModelStructs.h"
#include "../Vertex.h"
#include <vector>
#include <sstream>
#include <mutex>
#include "ReadSTLChunk.h"


class IDataAccess
{
public:
	virtual ~IDataAccess() = default;
	virtual bool LoadTerrain(const wchar_t*, std::vector<VertexMesh>&) = 0;
    virtual bool LoadCameraTrajectory(const wchar_t*, std::vector<CameraPose>&) = 0;
};

class TextFileDataAccess : public IDataAccess {

private:
    bool CreateVertex(VertexMesh& vertex, const std::string& line, std::vector<VertexMesh>& vertices, STLLineType& type, int& vertexCount);
    bool CreateCameraPose(CameraPose& cameraPose, const std::string& line, const std::vector<std::string>& headers);

public:
    TextFileDataAccess() = default;
    TextFileDataAccess(const TextFileDataAccess&) = delete;
    TextFileDataAccess& operator=(const TextFileDataAccess&) = delete;
    ~TextFileDataAccess() = default;
    bool LoadTerrain(const wchar_t* filename, std::vector<VertexMesh>& vertices) override;
    bool LoadCameraTrajectory(const wchar_t* filename, std::vector<CameraPose>& cameraPoses) override;
};

class TextFileDataAccessAsync : public IDataAccess {

    friend class ReadSTLChunk;

private:
    const int minChunkSize = 2629781;
    std::mutex m_mutex;
    std::vector<Facet> m_faces;
private:
    void ReadChunk(const std::wstring& filepath, int start, int end);
    bool ReadFile(const std::wstring& filepath, ICallableCreatorPtr creator);
    bool CreateFacet(Facet& facet, const std::string& line, STLLineType& type, int& vertexCount);
    int GetNumThreads(int fileSize);

public:
    TextFileDataAccessAsync() = default;
    TextFileDataAccessAsync(const TextFileDataAccessAsync&) = delete;
    TextFileDataAccessAsync& operator=(const TextFileDataAccessAsync&) = delete;
    ~TextFileDataAccessAsync() = default;
    bool LoadTerrain(const wchar_t* filename, std::vector<VertexMesh>& vertices) override;
    bool LoadCameraTrajectory(const wchar_t*, std::vector<CameraPose>& cameraPoses) override;
};
#endif // ! PERSISTENCE_H
