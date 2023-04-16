#ifndef I_PIXEL_SHADER
#define I_PIXEL_SHADER

#include <d3d11.h>
#include <DirectXMath.h>
#include "Light.h"
#include <wrl/client.h>
#include <memory>

class IPixelShader
{
public:
	virtual ~IPixelShader() = default;
	virtual bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>, HWND) = 0;
	virtual void Shutdown() = 0;
	virtual bool Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext>, int vertexCount, const Light&) = 0;

	virtual Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPixelShader(void) = 0;
};

typedef std::shared_ptr<IPixelShader> IPixelShaderPtr;
#endif // !I_PIXEL_SHADER