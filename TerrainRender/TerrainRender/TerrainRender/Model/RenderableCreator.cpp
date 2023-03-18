#include "RenderableCreator.h"
#include "PolygonMesh.h"
#include "Polygon.h"

IRenderable<Vertex>* PolygonMeshCreator::CreateRenderable() const
{
	return new PolygonMesh;
}

IRenderable<VertexPolygon>* PolygonCreator::CreateRenderable() const
{
	return new PolygonLine;
}

