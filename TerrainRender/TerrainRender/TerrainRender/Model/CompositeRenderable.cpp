#include "CompositeRenderable.h"
#include <algorithm>
#include "Light.h"
#include "PolygonMesh.h"
bool CompositeRenderable::Initialize(ID3D11Device* device, VertexShader* vertexShader, PixelShader* pixelShader, Vertex* vertices, UINT indexCount)
{
	if (device == nullptr || vertexShader == nullptr || pixelShader == nullptr)
	{
		return false;
	}

	this->m_device = device;
	this->m_vertexShader = vertexShader;
	this->m_pixelShader = pixelShader;
}

void CompositeRenderable::Shutdown() 
{
	for (IRenderable* renderable : m_renderables)
	{
		if (renderable)
		{
			renderable->Shutdown();
			delete renderable;
		}
	}
}

void CompositeRenderable::Render(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat,const Light& light)
{
	DirectX::XMMATRIX mat = DirectX::XMMatrixIdentity();

	for (IRenderable* renderable : m_renderables)
	{
		renderable->Render(deviceContext, worldMat, viewMat, projectionMat, light);
	}
}

bool CompositeRenderable::Add(Vertex* vertices, UINT indexCount)
{
	IRenderable* renderable = new PolygonMesh;
	if (!renderable)
	{
		return false;
	}
	renderable->Initialize(this->m_device, m_vertexShader, m_pixelShader, vertices, indexCount);
	this->Add(renderable);
}

bool CompositeRenderable::Add(IRenderable* renderable)
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
bool CompositeRenderable::Remove(IRenderable* renderable)
{
	if (std::find(this->m_renderables.begin(), this->m_renderables.end(), renderable) != this->m_renderables.end())
	{
		std::remove(this->m_renderables.begin(), this->m_renderables.end(), renderable);
		return true;
	}
	return false;
}