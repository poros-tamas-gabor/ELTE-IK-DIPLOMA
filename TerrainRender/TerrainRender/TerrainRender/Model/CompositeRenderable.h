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
	std::wstring						m_name;
	std::vector<IRendarablePtr<V>>		m_renderables;
	IVertexShader*						m_vertexShader = NULL;
	IPixelShader*						m_pixelShader = NULL;
	ID3D11Device*						m_device = NULL;
	bool								m_isSeen = true;

private:
	bool Add(IRendarablePtr<V> renderable)
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
		for (IRendarablePtr<V> renderable : m_renderables)
		{
			if (renderable)
			{
				renderable->Shutdown();
				//delete renderable;
			}
		}
	}
	void Render(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat, const Light& light) override
	{
		for (IRendarablePtr<V> renderable : m_renderables)
		{
			renderable->Render(deviceContext, worldMat, viewMat, projectionMat, light);
		}
	}
	bool Add(V* vertices, UINT indexCount, const IRenderableCreator<V>& renderableCreator, const std::wstring& renderableName)
	{
		IRendarablePtr<V> renderable = renderableCreator.CreateRenderable();
		if (!renderable)
		{
			return false;
		}
		renderable->SetName(renderableName);
		renderable->Initialize(this->m_device, m_vertexShader, m_pixelShader, vertices, indexCount);
		return this->Add(renderable);
	}

	bool Remove(IRendarablePtr<V> renderable)
	{
		if (std::find(this->m_renderables.begin(), this->m_renderables.end(), renderable) != this->m_renderables.end())
		{
			std::remove(this->m_renderables.begin(), this->m_renderables.end(), renderable);
			return true;
		}
		return false;
	}

	void SetName(const std::wstring& name) override
	{
		m_name = name;
	}
	void Rotate(float yaw, float pitch, float roll) override
	{
		for (IRendarablePtr<V> renderable : m_renderables)
		{
			renderable->Rotate(yaw, pitch, roll);
		}
	}
	void Translate(float x, float y, float z) override
	{
		for (IRendarablePtr<V> renderable : m_renderables)
		{
			renderable->Translate(x, y, z);
		}
	}
	void Scale(float x, float y, float z) override
	{
		for (IRendarablePtr<V> renderable : m_renderables)
		{
			renderable->Scale(x, y, z);
		}
	}

	DirectX::XMMATRIX GetWorldMatrix(void)  override
	{
		//TODO 
		return DirectX::XMMatrixIdentity();
	}

	void ResetTransformation() override
	{
		for (IRendarablePtr<V> renderable : m_renderables)
		{
			renderable->ResetTransformation();
		}
	}
	
	void RotateComponent(unsigned componentID, float pitch, float yaw,  float roll)
	{
		for (IRendarablePtr<V> renderable : m_renderables)
		{
			if(renderable->GetID() == componentID)
				renderable->Rotate(pitch, yaw,  roll);
		}
	}
	void TranslateComponent(unsigned componentID, float x, float y, float z)
	{
		for (IRendarablePtr<V> renderable : m_renderables)
		{
			if (renderable->GetID() == componentID)
				renderable->Translate(x, y, z);
		}
	}
	void ScaleComponent(unsigned componentID, float x, float y, float z)
	{
		for (IRendarablePtr<V> renderable : m_renderables)
		{
			if (renderable->GetID() == componentID)
				renderable->Scale(x, y, z);
		}
	}

	IRenderableState	GetState(void) const override {
		return IRenderableState();
	}

	void CollectIRenderableState(std::vector<IRenderableState>& vector)
	{
		for (IRendarablePtr<V> renderable : m_renderables)
		{
			IRenderableState info = renderable->GetState();
			vector.push_back(info);
		}
	}
	std::wstring GetName(void)
	{
		return m_name;
	}

	void ClearRenderables(void)
	{
		this->Shutdown();
		this->m_renderables.clear();
	}

	IRendarablePtr<V> GetLastAddedComponent(void)
	{
		return m_renderables.back();
	}

	void SetColor(float r, float g, float b, float a) override{}

	void SetColorComponent(unsigned componentID, float r, float g, float b, float a)
	{
		for (IRendarablePtr<V> renderable : m_renderables)
		{
			if (renderable->GetID() == componentID)
				renderable->SetColor( r,g,b,a);
		}
	}

	void SetColorComponent(std::wstring componentName, float r, float g, float b, float a)
	{
		for (IRendarablePtr<V> renderable : m_renderables)
		{
			if (renderable->GetName().find(componentName) != std::wstring::npos)
			{
				renderable->SetColor(r, g, b, a);
			}

		}
	}

	void SetIsSeen(bool m_isSeen) override 
	{
		m_isSeen = m_isSeen;
		for (IRendarablePtr<V> renderable : m_renderables)
		{
			renderable->SetIsSeen(m_isSeen);
		}
	}

	bool IsSeen(void) const override
	{
		return m_isSeen;
	}

	void SetIsSeenComponent(unsigned componentID, bool m_isSeen)
	{
		for (IRendarablePtr<V> renderable : m_renderables)
		{
			if (renderable->GetID() == componentID)
				renderable->SetIsSeen(m_isSeen);
		}
	}
};
#endif