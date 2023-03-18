#ifndef COMPOSITE_MESH_H
#define COMPOSITE_MESH_H

#include "IRenderable.h"
#include <algorithm>
#include "Light.h"
#include <vector>
#include "IRenderableCreator.h"


template <class V>
class CompositeRenderable : public IRenderable<V>
{
private:
	std::vector<IRenderable<V>*>	m_renderables;
	IVertexShader*					m_vertexShader = NULL;
	IPixelShader*					m_pixelShader = NULL;
	ID3D11Device*					m_device = NULL;

private:
	bool Add(IRenderable<V>* renderable)
	{
		if (std::find(this->m_renderables.begin(), this->m_renderables.end(), renderable) != this->m_renderables.end())
		{
			return false;
		}
		else {
			this->m_renderables.push_back(renderable);
			return true;
		}
	}

public:

	virtual ~CompositeRenderable() = default;
	bool Initialize(ID3D11Device* device, IVertexShader* vertexShader, IPixelShader* pixelShader, V* vertices, UINT indexCount) override
	{
		if (device == nullptr || vertexShader == nullptr || pixelShader == nullptr)
		{
			return false;
		}

		this->m_device = device;
		this->m_vertexShader = vertexShader;
		this->m_pixelShader = pixelShader;
		return true;
	}
	void Shutdown() override
	{
		for (IRenderable<V>* renderable : m_renderables)
		{
			if (renderable)
			{
				renderable->Shutdown();
				delete renderable;
			}
		}
	}
	void Render(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat, const Light& light) override
	{
		DirectX::XMMATRIX mat = DirectX::XMMatrixIdentity();

		for (IRenderable<V>* renderable : m_renderables)
		{
			renderable->Render(deviceContext, worldMat, viewMat, projectionMat, light);
		}
	}
	bool Add(V* vertices, UINT indexCount, const IRenderableCreator<V>& polygonCreator)
	{
		IRenderable<V>* renderable = polygonCreator.CreateRenderable();
		if (!renderable)
		{
			return false;
		}
		renderable->Initialize(this->m_device, m_vertexShader, m_pixelShader, vertices, indexCount);
		return this->Add(renderable);
	}

	bool Remove(IRenderable<V>* renderable)
	{
		if (std::find(this->m_renderables.begin(), this->m_renderables.end(), renderable) != this->m_renderables.end())
		{
			std::remove(this->m_renderables.begin(), this->m_renderables.end(), renderable);
			return true;
		}
		return false;
	}
};
#endif