#ifndef I_RENDERABLE_H
#define I_RENDERABLE_H

///////////////////////////////////////////////////////////////////////////////
// IRenderable.h
// ==============
//
// The IRenderableBase class ensures that every renderable object has a unique identifier.
//
// The IRenderable interface is used in a program to display graphical elements
// and contains a template parameter V that determines the type of the vertex points.
//
// The IRenderable interface contains pure virtual functions that must be implemented by the derived class.
// The derived class has the responsibility to allocate the memory for the vertex and index buffer via DirectX calls.
//
// The order of transformation matrices applied is fixed: scaling, then rotation, then translation.
//
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

#include <d3d11.h>
#include "Light.h"
#include "IPixelShader.h"
#include "IVertexShader.h"
#include "Persistence/ModelStructs.h"
#include <string>
#include <memory>
#include <wrl/client.h>



class IRenderableBase
{
private:
	static unsigned			nextID;
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

	virtual bool Initialize(Microsoft::WRL::ComPtr <ID3D11Device> device, IVertexShaderPtr, IPixelShaderPtr, V* vertices, unsigned long* indices, UINT vertexCount, UINT indexCount ) = 0;
	virtual void Shutdown() = 0;
	virtual void Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext>, DirectX::XMMATRIX worldMat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat,const Light& light) = 0;

	virtual void Rotate(float pitch, float yaw, float roll) = 0;
	virtual void Translate(float x, float y, float z) = 0;
	virtual void Scale(float x, float y, float z) = 0;
	virtual void ResetTransformation() = 0;

	virtual void SetName(const std::wstring& name) = 0;
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