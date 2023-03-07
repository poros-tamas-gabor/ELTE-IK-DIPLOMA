#ifndef PERSISTENCE_H
#define	PERSISTENCE_H
#include "ModelVertex.h"
#include <vector>
#include <sstream>


class IDataAccess
{
public:
	virtual ~IDataAccess() = default;
	virtual bool LoadTerrain(const wchar_t* filename, std::vector<ModelVertex>& vertices) = 0;
};

class FileDataAccess : IDataAccess
{

private:
	void CreateVertex(ModelVertex& vertex, std::stringstream& sstream, std::vector<ModelVertex>& vertices);


public:
	FileDataAccess() = default;
	FileDataAccess(const FileDataAccess&) = delete;
	FileDataAccess&  operator=(const FileDataAccess&) = delete;
	~FileDataAccess() = default;
	bool LoadTerrain(const wchar_t* filename, std::vector<ModelVertex>& vertices) override;
};
#endif // ! PERSISTENCE_H

