#ifndef COMPOSITE_MESH_H
#define COMPOSITE_MESH_H

#include "IRenderable.h"
#include <algorithm>
#include "Light.h"
#include <vector>
#include "IRenderableCreator.h"
#include "../TRException.h"


template <class V>
class CompositeRenderable : public IRenderable<V>
{
private:
	std::wstring							m_name;
	Vector3D								m_rotation = { 0,0,0 };
	Vector3D								m_scaling = { 1, 1, 1 };
	Vector3D								m_translation = { 0,0,0 };
	std::vector<IRendarablePtr<V>>			m_renderables;
	IVertexShaderPtr						m_vertexShader = NULL;
	IPixelShaderPtr							m_pixelShader = NULL;
	Microsoft::WRL::ComPtr<ID3D11Device>	m_device = NULL;
	bool									m_isSeen = true;

private:
	bool Add(IRendarablePtr<V> renderable)
	{
		if (std::find(this->m_renderables.begin(), this->m_renderables.end(), renderable) != this->m_renderables.end())
		{
			return false;
		}
		else {
			this->m_renderables.push_back(renderable);

			renderable->Rotate(m_rotation.x, m_rotation.y, m_rotation.z);
			renderable->Translate(m_translation.x, m_translation.y, m_translation.z);
			renderable->Scale(m_scaling.x, m_scaling.y, m_scaling.z);

			return true;
		}
	}
	void RotateComponent(unsigned componentID, float pitch, float yaw, float roll)
	{
		for (IRendarablePtr<V> renderable : m_renderables)
		{
			if (renderable->GetID() == componentID)
				renderable->Rotate(pitch, yaw, roll);
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

public:

	virtual ~CompositeRenderable() = default;

	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, IVertexShaderPtr vertexShader, IPixelShaderPtr pixelShader, V* vertices, unsigned long* indices, UINT vertexCount, UINT indexCount) override
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
	void Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, DirectX::XMMATRIX worldMat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat, const Light& light) override
	{
		for (IRendarablePtr<V> renderable : m_renderables)
		{
			renderable->Render(deviceContext, worldMat, viewMat, projectionMat, light);
		}
	}
	
	bool Add(V* vertices, unsigned long* indices, UINT vertexCount, UINT indexCount, const IRenderableCreator<V>& renderableCreator, const std::wstring& renderableName)
	{
		IRendarablePtr<V> renderable = renderableCreator.CreateRenderable();
		if (!renderable)
		{
			return false;
		}
		renderable->SetName(renderableName);

		renderable->Initialize(this->m_device, m_vertexShader, m_pixelShader, vertices, indices, vertexCount, indexCount);
		return this->Add(renderable);
	}

	bool Remove(IRendarablePtr<V> renderable)
	{
		if (std::find(this->m_renderables.begin(), this->m_renderables.end(), renderable) != this->m_renderables.end())
		{
			this->m_renderables.erase(std::remove(this->m_renderables.begin(), this->m_renderables.end(), renderable));
			return true;
		}
		return false;
	}

	void SetName(const std::wstring& name) override
	{
		m_name = name;
	}
	void Rotate(float pitch, float yaw, float roll) override
	{
		m_rotation = { pitch, yaw, roll };
		for (IRendarablePtr<V> renderable : m_renderables)
		{
			renderable->Rotate(pitch, yaw, roll);
		}
	}
	void Translate(float x, float y, float z) override
	{
		m_translation = { x,y,z };
		for (IRendarablePtr<V> renderable : m_renderables)
		{
			renderable->Translate(x, y, z);
		}
	}
	void Scale(float x, float y, float z) override
	{
		m_scaling = { x, y, z };
		for (IRendarablePtr<V> renderable : m_renderables)
		{
			renderable->Scale(x, y, z);
		}
	}

	DirectX::XMMATRIX GetWorldMatrix(void)  override
	{
		return DirectX::XMMatrixIdentity();
	}

	void ResetTransformation() override
	{
		for (IRendarablePtr<V> renderable : m_renderables)
		{
			renderable->ResetTransformation();
		}
	}
	
	IRenderableState	GetState(void) const override {
		IRenderableState state;
		state.id = this->GetID();
		state.m_isSeen = m_isSeen;
		state.name = this->m_name;
		state.rotation = this->m_rotation;
		state.scale = this->m_scaling;
		state.translation = this->m_translation;

		return state;
	}

	void CollectIRenderableState(std::vector<IRenderableState>& vector) const
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
		THROW_TREXCEPTION_IF_FAILED(!m_renderables.empty(), L"Failed to get last element because Renderables are empty");
		return m_renderables.back();
	}

	void SetColor(float r, float g, float b, float a) override
	{
		for (IRendarablePtr<V> renderable : m_renderables)
		{
			renderable->SetColor(r, g, b, a);
		}
	}

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