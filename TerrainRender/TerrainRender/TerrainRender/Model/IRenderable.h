#ifndef I_RENDERABLE_H
#define I_RENDERABLE_H

#include <d3d11.h>
#include "Light.h"
#include "IPixelShader.h"
#include "IVertexShader.h"

template <class V>
class IRenderable
{
public:

	virtual ~IRenderable() = default;
	virtual bool Initialize(ID3D11Device* device, IVertexShader*, IPixelShader*, V* vertices, UINT indexCount) = 0;
	virtual void Shutdown() = 0;
	virtual void Render(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat,const Light& light) = 0;
};

#endif