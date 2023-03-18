#ifndef POLYGON_VERTEX_H
#define POLYGON_VERTEX_H
#include <DirectXMath.h>


struct Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT4 color;
};
#endif