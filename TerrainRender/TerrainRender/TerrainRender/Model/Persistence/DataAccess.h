#ifndef PERSISTENCE_H
#define	PERSISTENCE_H
#include "ModelStructs.h"
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

class BinaryFileDataAccessAsync : public IDataAccess {

    friend class ReadSTLChunk;

private:
    const int minChunkSize = 50000;
    std::vector<Facet> m_faces;
private:
    bool ReadFile(const std::wstring& filepath);
    int GetNumThreads(int fileSize);
    bool CreateCameraPose(CameraPose& cameraPose, const std::string& line, const std::vector<std::string>& headers);

public:
    BinaryFileDataAccessAsync() = default;
    BinaryFileDataAccessAsync(const BinaryFileDataAccessAsync&) = delete;
    BinaryFileDataAccessAsync& operator=(const BinaryFileDataAccessAsync&) = delete;
    ~BinaryFileDataAccessAsync() = default;
    bool LoadTerrain(const wchar_t* filename, std::vector<VertexMesh>& vertices) override;
    bool LoadCameraTrajectory(const wchar_t*, std::vector<CameraPose>& cameraPoses) override;
};
#endif // ! PERSISTENCE_H
