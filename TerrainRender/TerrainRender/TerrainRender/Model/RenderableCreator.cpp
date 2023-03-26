#include "RenderableCreator.h"
#include "PolygonMesh.h"
#include "LineList.h"
#include "PolyLine.h"

IRendarablePtr<VertexMesh> PolygonMeshCreator::CreateRenderable() const
{
	return   std::make_shared<PolygonMesh>();
}

IRendarablePtr<VertexPolyLine> LineListCreator::CreateRenderable() const
{
	return std::make_shared<LineList>();
}

IRendarablePtr<VertexPolyLine> PolyLineCreator::CreateRenderable() const
{
	return std::make_shared<PolyLine>(); 
}


