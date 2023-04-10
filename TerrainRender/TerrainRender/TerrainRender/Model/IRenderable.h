#ifndef I_RENDERABLE_H
#define I_RENDERABLE_H

#include <d3d11.h>
#include "Light.h"
#include "IPixelShader.h"
#include "IVertexShader.h"
#include "Persistence/ModelStructs.h"
#include <string>
#include <memory>

//The order of transformation matrices applied is fixed: scaling, then rotation, then translation.

class IRenderableBase
{
private:
	inline static unsigned	nextID = 0;
	unsigned				m_ID;
public:
	IRenderableBase() : m_ID(++nextID) {}
	virtual ~IRenderableBase() = default;
	unsigned GetID() const { return m_ID; }
};

template <class V>
class IRenderable : public IRenderableBase
{
public:
	IRenderable()			= default;
	virtual ~IRenderable()	= default;
	virtual bool Initialize(ID3D11Device* device, IVertexShader*, IPixelShader*, V* vertices, unsigned long* indices, UINT vertexCount, UINT indexCount ) = 0;
	virtual void Shutdown() = 0;
	virtual void Render(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat,const Light& light) = 0;

	virtual void SetName(const std::wstring& name) = 0;
	virtual void Rotate(float pitch, float yaw, float roll) = 0;
	virtual void Translate(float x, float y, float z) = 0;
	virtual void Scale(float x, float y, float z) = 0;
	virtual void ResetTransformation() = 0;
	virtual void SetColor(float r, float g, float b, float a) = 0;
	virtual void SetIsSeen(bool m_isSeen) = 0;
	virtual bool IsSeen(void) const = 0;

	virtual IRenderableState	GetState(void) const = 0;
	virtual std::wstring		GetName(void) = 0;
	virtual DirectX::XMMATRIX	GetWorldMatrix(void) = 0;
};

template <class V>
using IRendarablePtr = std::shared_ptr<IRenderable<V>>;
#endif