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

	virtual bool LoadTerrainSharpEdges(const wchar_t*) = 0;
    //Use After LoadTerrainSharpEdges
    virtual const std::vector<stlFacet>& GetFacets(void) = 0;



    virtual bool LoadCameraTrajectory(const wchar_t*, std::vector<CameraPose>&) = 0;
    virtual bool LoadParameterFile(const wchar_t*, ParameterFile& params) = 0;

	virtual bool LoadTerrainSoftEdges(const wchar_t*) = 0;
    //Use After LoadTerrainSoftEdges
    virtual const std::vector<StlVertex>& GetSolidVertices() = 0;
    //Use After LoadTerrainSoftEdges
    virtual const std::vector<FacetCornerIndices>& GetSolidIndices() = 0;
};

class BinaryFileDataAccessAsync : public IDataAccess {

    friend class ReadSTLChunkSharp;

private:
    const int minChunkSize = 50000;
    std::vector<stlFacet> m_faces;
    std::vector<StlVertex> m_vertices;
    std::vector<FacetCornerIndices> m_indices;
    std::mutex m_mutex_hashtable;
    std::mutex m_mutex_indices;
    std::mutex m_mutex_vertices;

private:
    bool ReadFileSharpEdges(const std::wstring& filepath);
    bool ReadFileSolid(const std::wstring& filepath);
    int  GetNumThreads(int fileSize);
    bool CreateCameraPose(CameraPose& cameraPose, const std::string& line, const std::vector<std::string>& headers);

public:
    BinaryFileDataAccessAsync() = default;
    BinaryFileDataAccessAsync(const BinaryFileDataAccessAsync&) = delete;
    BinaryFileDataAccessAsync& operator=(const BinaryFileDataAccessAsync&) = delete;
    ~BinaryFileDataAccessAsync() = default;

    bool LoadTerrainSharpEdges(const wchar_t* filename) override;
    bool LoadTerrainSoftEdges(const wchar_t* filename) override;
    bool LoadCameraTrajectory(const wchar_t*, std::vector<CameraPose>& cameraPoses) override;
    bool LoadParameterFile(const wchar_t*, ParameterFile& params) override;
    
    const std::vector<stlFacet>& GetFacets(void);
    const std::vector<StlVertex>& GetSolidVertices() override;
    const std::vector<FacetCornerIndices>& GetSolidIndices() override;
};
#endif // ! PERSISTENCE_H
