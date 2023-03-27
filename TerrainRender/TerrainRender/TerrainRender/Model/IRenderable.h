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
template <class V>
class IRenderable
{
private:
	//TODO: nextID 
	inline static unsigned nextID = rand() % 10000;
	unsigned ID;

public:
	IRenderable() : ID(++nextID) {}
	virtual ~IRenderable() = default;
	unsigned GetID() { return ID; }

	virtual bool Initialize(ID3D11Device* device, IVertexShader*, IPixelShader*, V* vertices, UINT indexCount) = 0;
	virtual void Shutdown() = 0;
	virtual void Render(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat,const Light& light) = 0;

	virtual void SetName(const std::wstring& name) = 0;
	virtual std::wstring GetName(void) = 0;
	virtual void Rotate(float pitch, float yaw, float roll) = 0;
	virtual void Translate(float x, float y, float z) = 0;
	virtual void Scale(float x, float y, float z) = 0;
	virtual void ResetTransformation() = 0;
	virtual DirectX::XMMATRIX GetLocalMatrix(void) = 0;
};

template <class V>
using IRendarablePtr = std::shared_ptr<IRenderable<V>>;
#endif