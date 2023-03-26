#ifndef RENDERABLE_CREATOR
#define RENDERABLE_CREATOR

#include "IRenderableCreator.h"

class PolygonMeshCreator : public IRenderableCreator<VertexMesh>
{
public:
	virtual IRendarablePtr<VertexMesh> CreateRenderable() const override;
};


class LineListCreator : public IRenderableCreator<VertexPolyLine>
{
public:
	virtual  IRendarablePtr<VertexPolyLine> CreateRenderable() const override;
};

class PolyLineCreator : public IRenderableCreator<VertexPolyLine>
{
public:
	virtual IRendarablePtr<VertexPolyLine> CreateRenderable() const override;
};

#endif // !POLYGON_MESH_CREATOR

