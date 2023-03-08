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

class TextFileDataAccess : public IDataAccess
{

private:
	void CreateVertex(ModelVertex& vertex, std::stringstream& sstream, std::vector<ModelVertex>& vertices);


public:
	TextFileDataAccess() = default;
	TextFileDataAccess(const TextFileDataAccess&) = delete;
	TextFileDataAccess&  operator=(const TextFileDataAccess&) = delete;
	~TextFileDataAccess() = default;
	bool LoadTerrain(const wchar_t* filename, std::vector<ModelVertex>& vertices) override;
};
#endif // ! PERSISTENCE_H

