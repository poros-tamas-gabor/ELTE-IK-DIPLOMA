#ifndef I_VERTEX_SHADER
#define I_VERTEX_SHADER

///////////////////////////////////////////////////////////////////////////////
// IVertexShader.h
// ===============
// 
// The IVertexShader interface defines the methods that every vertex shader class should implement.
// By creating a class that derives from the IVertexShader interface and implements its methods, 
// you can define a custom vertex shader and use it in graphics pipeline of the program.
//
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <memory>
#include "Light.h"

class IVertexShader
{
public:
	virtual ~IVertexShader() = default;
	virtual bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>) = 0;
	virtual void Shutdown() = 0;
	virtual bool Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext>, DirectX::XMMATRIX worldmat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat, DirectX::XMFLOAT4 color) = 0;

	virtual Microsoft::WRL::ComPtr<ID3D11VertexShader> GetVertexShader(void) = 0;
	virtual Microsoft::WRL::ComPtr<ID3D11InputLayout> GetInputLayout(void) = 0;
};

typedef std::shared_ptr<IVertexShader> IVertexShaderPtr;
#endif // !I_VERTEX_SHADER

