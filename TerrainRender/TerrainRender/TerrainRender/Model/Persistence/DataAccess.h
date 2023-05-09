#ifndef PERSISTENCE_H
#define	PERSISTENCE_H
#include "ModelStructs.h"
#include <vector>
#include <sstream>
#include <mutex>
#include <memory>
#include "ReadSTLChunk.h"

///////////////////////////////////////////////////////////////////////////////
// IDataAccess.h
// ============
// The IDataAccess interface defines the contract for the Data Access component in the Model-View-Controller (MVC) architecture.
//
// The IDataAccess interface is responsible for managing the persistence of data by defining methods that allow the Model to access and manipulate the data.
//
// BinaryFileDataAccessAsync is a derived class of IDataAccess. 
// The purpose of this class is to asynchronously load meshes from binary STL files, as well as open the trajectory CSV file and configuration file.
//
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

class IDataAccess
{
public:
	virtual ~IDataAccess() = default;

	virtual void LoadTerrain_withSharpEdges(const wchar_t*) = 0;
    //Use After LoadTerrain_withSharpEdges
    virtual const std::vector<StlFacet>& GetFacets(void) = 0;

    virtual void LoadCameraTrajectory(const wchar_t*, std::vector<CameraPose>&) = 0;
    virtual void LoadConfigurationFile(const wchar_t*, ParameterFile& params) = 0;

	virtual void LoadTerrain_withSoftEdges(const wchar_t*) = 0;
    //Use After LoadTerrain_withSoftEdges
    virtual const std::vector<StlVertex>& GetVertices_Soft() = 0;
    //Use After LoadTerrain_withSoftEdges
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
    std::vector<StlFacet> m_facets;
    std::vector<StlVertex> m_vertices;
    std::vector<CornerIndices> m_indices;
    std::shared_mutex m_mutex_hashtable;

private:
    unsigned GetNumTriangles_stlBin(const std::wstring& filepath);
    void ReadFileSharpEdges(const std::wstring& filepath);
    void ReadFileSoftEdges(const std::wstring& filepath);
    int  GetNumThreads(int numOfFacets);
    bool CreateCameraPose(CameraPose& cameraPose, const std::string& line, const std::vector<std::string>& headers);
    bool CheckTrajectoryHeaders(const std::vector<std::string>& headers);

public:
    BinaryFileDataAccessAsync() = default;
    BinaryFileDataAccessAsync(const BinaryFileDataAccessAsync&) = delete;
    BinaryFileDataAccessAsync& operator=(const BinaryFileDataAccessAsync&) = delete;
    ~BinaryFileDataAccessAsync() = default;

    void LoadTerrain_withSharpEdges(const wchar_t* filename) override;
    void LoadTerrain_withSoftEdges(const wchar_t* filename) override;
    void LoadCameraTrajectory(const wchar_t*, std::vector<CameraPose>& cameraPoses) override;
    void LoadConfigurationFile(const wchar_t*, ParameterFile& params) override;
    
    const std::vector<StlFacet>& GetFacets(void);
    const std::vector<StlVertex>& GetVertices_Soft() override;
    const std::vector<CornerIndices>& GetIndices_Soft() override;
};
#endif // ! PERSISTENCE_H
