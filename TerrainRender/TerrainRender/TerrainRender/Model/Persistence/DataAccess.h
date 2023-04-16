#ifndef PERSISTENCE_H
#define	PERSISTENCE_H
#include "ModelStructs.h"
#include <vector>
#include <sstream>
#include <mutex>
#include <memory>
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
    virtual const std::vector<StlVertex>& GetVertices_Soft() = 0;
    //Use After LoadTerrainSoftEdges
    virtual const std::vector<CornerIndices>& GetIndices_Soft() = 0;
};

typedef std::shared_ptr<IDataAccess> IDataAccessPtr;

class BinaryFileDataAccessAsync : public IDataAccess {

    friend class ReadSTLChunkSharp;

private:
    const unsigned MIN_NUM_OF_FACETS_IN_CHUNK = 50000;
    const unsigned STL_BIN_HEADER_SIZE = 80;
    const unsigned STL_BIN_NUM_OF_TRIANGLE_SIZE = 4;
    const unsigned STL_BIN_TRIANGLE_SIZE = 50;
    std::vector<stlFacet> m_facets;
    std::vector<StlVertex> m_vertices;
    std::vector<CornerIndices> m_indices;
    std::shared_mutex m_mutex_hashtable;

private:
    unsigned GetNumTriangles_stlBin(const std::wstring& filepath);
    bool ReadFileSharpEdges(const std::wstring& filepath);
    bool ReadFileSoftEdges(const std::wstring& filepath);
    int  GetNumThreads(int numOfFacets);
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
    const std::vector<StlVertex>& GetVertices_Soft() override;
    const std::vector<CornerIndices>& GetIndices_Soft() override;
};
#endif // ! PERSISTENCE_H
