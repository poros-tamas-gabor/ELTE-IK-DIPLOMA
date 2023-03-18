#ifndef I_PIXEL_SHADER
#define I_PIXEL_SHADER

#include <d3d11.h>
#include <DirectXMath.h>
#include "Light.h"

class IPixelShader
{
public:
	virtual ~IPixelShader() = default;
	virtual bool Initialize(ID3D11Device*, HWND) = 0;
	virtual void Shutdown() = 0;
	virtual bool Render(ID3D11DeviceContext*, int vertexCount) = 0;

	virtual ID3D11PixelShader* GetPixelShader(void) = 0;
};
#endif // !I_PIXEL_SHADER