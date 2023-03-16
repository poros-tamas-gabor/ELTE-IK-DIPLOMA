#ifndef COMPOSITE_RENDERABLE_H
#define COMPOSITE_RENDERABLE_H

#include "IRenderable.h"
#include <vector>

class VertexShader;
class PixelShader;

class CompositeRenderable : public IRenderable
{
private:
	std::vector<IRenderable*>	m_renderables;
	VertexShader* m_vertexShader;
	PixelShader* m_pixelShader;
	ID3D11Device* m_device;
	bool Add(IRenderable*);

public:

	virtual ~CompositeRenderable() = default;
	bool Initialize(ID3D11Device* device, VertexShader*, PixelShader*, Vertex* vertices, UINT indexCount) override;
	void Shutdown() override;
	void Render(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat,const Light& light) override;
	bool Add(Vertex* vertices, UINT indexCount);
	bool Remove(IRenderable*);
};
#endif