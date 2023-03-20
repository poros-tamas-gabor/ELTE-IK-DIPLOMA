#ifndef RENDERABLE_CREATOR
#define RENDERABLE_CREATOR

#include "IRenderableCreator.h"
#include  "Vertex.h"
#include  "VertexPolygon.h"
class PolygonMeshCreator : public IRenderableCreator<Vertex>
{
public:
	virtual IRenderable<Vertex>* CreateRenderable() const override;
};


class PolygonCreator : public IRenderableCreator<VertexPolygon>
{
public:
	virtual IRenderable<VertexPolygon>* CreateRenderable() const override;

};

#endif // !POLYGON_MESH_CREATOR

