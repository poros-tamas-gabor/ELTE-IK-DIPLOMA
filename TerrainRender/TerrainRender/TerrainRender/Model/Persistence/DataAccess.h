#ifndef PERSISTENCE_H
#define	PERSISTENCE_H
#include "ModelStructs.h"
#include "../Vertex.h"
#include <vector>
#include <sstream>


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
#endif // ! PERSISTENCE_H
