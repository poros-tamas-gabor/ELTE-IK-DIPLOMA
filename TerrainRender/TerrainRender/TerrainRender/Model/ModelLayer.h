#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "Persistence/ModelVertex.h"
#include "Persistence/DataAccess.h"


class ModelLayer
{
private:
	FileDataAccess					_persistence;
	std::vector<ModelVertex>		_vertices;

public:
	ModelLayer();
	bool LoadTerrain(const wchar_t* filepath);
	void GetVertices(std::vector<ModelVertex>& vertices) const;
};
#endif // !MODEL_H

