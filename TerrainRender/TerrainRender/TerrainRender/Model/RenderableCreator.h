#ifndef RENDERABLE_CREATOR
#define RENDERABLE_CREATOR

#include "IRenderableCreator.h"
#include  "Vertex.h"
#include  "VertexPolygon.h"
class PolygonMeshCreator : public IRenderableCreator<VertexMesh>
{
public:
	virtual IRenderable<VertexMesh>* CreateRenderable() const override;
};


class LineListCreator : public IRenderableCreator<VertexPolyLine>
{
public:
	virtual IRenderable<VertexPolyLine>* CreateRenderable() const override;
};

class PolyLineCreator : public IRenderableCreator<VertexPolyLine>
{
public:
	virtual IRenderable<VertexPolyLine>* CreateRenderable() const override;
};

#endif // !POLYGON_MESH_CREATOR

