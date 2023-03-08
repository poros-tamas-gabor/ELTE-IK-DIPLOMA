#ifndef GFX_VERTEX_H
#define GFX_VERTEX_H


#include <DirectXMath.h>

struct GfxVertex
{
	GfxVertex() = default;;
	//GfxVertex(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 normal, DirectX::XMFLOAT4 color) : position(position), normal(normal), color(color) {}
	GfxVertex(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 color) : position(position), color(color) {}

	DirectX::XMFLOAT3 position;
	//DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT4 color;
};
#endif // !GFX_VERTEX_H
