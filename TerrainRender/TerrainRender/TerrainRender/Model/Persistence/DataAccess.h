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
	virtual bool LoadTerrain(const wchar_t*) = 0;
	virtual bool LoadTerrainSolid(const wchar_t*) = 0;
    virtual bool LoadCameraTrajectory(const wchar_t*, std::vector<CameraPose>&) = 0;
    virtual const std::vector<Facet>& GetFacets(void) = 0;
    virtual bool LoadParameterFile(const wchar_t*, ParameterFile& params) = 0;

    virtual const std::vector<StlVertex>& GetSolidVertices() = 0;
    virtual const std::vector<FacetIndices>& GetSolidIndices() = 0;
};

class BinaryFileDataAccessAsync : public IDataAccess {

    friend class ReadSTLChunk;

private:
    const int minChunkSize = 50000;
    std::vector<Facet> m_faces;

    std::vector<StlVertex> m_vertices;
    std::vector<FacetIndices> m_facets;
private:
    bool ReadFile(const std::wstring& filepath);
    bool ReadFileSolid(const std::wstring& filepath);
    int GetNumThreads(int fileSize);
    bool CreateCameraPose(CameraPose& cameraPose, const std::string& line, const std::vector<std::string>& headers);

public:
    BinaryFileDataAccessAsync() = default;
    BinaryFileDataAccessAsync(const BinaryFileDataAccessAsync&) = delete;
    BinaryFileDataAccessAsync& operator=(const BinaryFileDataAccessAsync&) = delete;
    ~BinaryFileDataAccessAsync() = default;
    bool LoadTerrain(const wchar_t* filename) override;
    bool LoadTerrainSolid(const wchar_t* filename) override;
    bool LoadCameraTrajectory(const wchar_t*, std::vector<CameraPose>& cameraPoses) override;
    bool LoadParameterFile(const wchar_t*, ParameterFile& params) override;
    const std::vector<Facet>& GetFacets(void);


   const std::vector<StlVertex>& GetSolidVertices() override;
   const std::vector<FacetIndices>& GetSolidIndices() override;
};
#endif // ! PERSISTENCE_H
