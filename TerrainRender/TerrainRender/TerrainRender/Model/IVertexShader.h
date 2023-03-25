#ifndef I_VERTEX_SHADER
#define I_VERTEX_SHADER

#include <d3d11.h>
#include <DirectXMath.h>
#include "Light.h"

class IVertexShader
{
public:
	virtual ~IVertexShader() = default;
	virtual bool Initialize(ID3D11Device*, HWND) = 0;
	virtual void Shutdown() = 0;
	virtual bool Render(ID3D11DeviceContext*, DirectX::XMMATRIX worldmat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat, const Light&) = 0;

	virtual ID3D11VertexShader* GetVertexShader(void) = 0;
	virtual ID3D11InputLayout* GetInputLayout(void) = 0;
};
#endif // !I_VERTEX_SHADER

