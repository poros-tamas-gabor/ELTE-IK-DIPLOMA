#include "ModelLayer.h"

ModelLayer::ModelLayer() 
{

}

bool ModelLayer::LoadTerrain(const wchar_t* filepath)
{
	return _persistence.LoadTerrain(filepath, this->_vertices);
}
void ModelLayer::GetVertices(std::vector<ModelVertex>& vertices) const
{
	vertices = this->_vertices;
}
