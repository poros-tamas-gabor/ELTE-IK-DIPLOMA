#include "RenderableCreator.h"
#include "PolygonMesh.h"
#include "LineList.h"
#include "PolyLine.h"

IRenderable<VertexMesh>* PolygonMeshCreator::CreateRenderable() const
{
	return new PolygonMesh;
}

IRenderable<VertexPolyLine>* LineListCreator::CreateRenderable() const
{
	return new LineList;
}

IRenderable<VertexPolyLine>* PolyLineCreator::CreateRenderable() const
{
	return new PolyLine;
}


