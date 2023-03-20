#include "RenderableCreator.h"
#include "PolygonMesh.h"
#include "LineList.h"

IRenderable<Vertex>* PolygonMeshCreator::CreateRenderable() const
{
	return new PolygonMesh;
}

IRenderable<VertexPolygon>* PolygonCreator::CreateRenderable() const
{
	return new LineList;
}



