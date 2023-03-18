#ifndef I_VERTEX_SHADER
#define I_VERTEX_SHADER

#include <d3d11.h>
#include <DirectXMath.h>
#include "Light.h"

class IVertexShader
{
	virtual ~IVertexShader() = default;
	virtual bool Initialize(ID3D11Device*, HWND) = 0;
	virtual void Shutdown() = 0;
	virtual bool Render(ID3D11DeviceContext*, DirectX::XMMATRIX worldmat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat, const Light& light) = 0;

ID3D11VertexShader* GetVertexShader(void);
ID3D11InputLayout* GetInputLayout(void);
};
#endif // !I_VERTEX_SHADER

