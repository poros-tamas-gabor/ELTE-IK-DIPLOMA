#ifndef I_PIXEL_SHADER
#define I_PIXEL_SHADER

///////////////////////////////////////////////////////////////////////////////
// IPixelShader.h
// ==============
// 
// The IPixelShader interface defines the methods that every pixel shader class should implement.
// By creating a class that derives from the IPixelShader interface and implements its methods, 
// you can define a custom pixel shader and use it in graphics pipeline of the program.
//
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

#include <d3d11.h>
#include <DirectXMath.h>
#include "Light.h"
#include <wrl/client.h>
#include <memory>

class IPixelShader
{
public:
	virtual ~IPixelShader() = default;
	virtual bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>) = 0;
	virtual void Shutdown() = 0;
	virtual bool Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext>, int vertexCount, const Light&) = 0;

	virtual Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPixelShader(void) = 0;
};

typedef std::shared_ptr<IPixelShader> IPixelShaderPtr;
#endif // !I_PIXEL_SHADER