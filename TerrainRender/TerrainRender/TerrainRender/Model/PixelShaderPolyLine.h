#ifndef PIXEL_SHADER_POLYLINE_H
#define	PIXEL_SHADER_POLYLINE_H

///////////////////////////////////////////////////////////////////////////////
// PixelShaderPolyline.h
// =====================
// 
// PixelShaderPolyline is a class derived from the IPixelShader interface.
// Its main purpose is to set up the pixel shader of the lines by 
// - compiling the pixel shader from the.hlsl file, 
// - allocating memory for the constant buffers, 
// - creating the DirectX pixel shader, 
// - and using the device context to render the pixel shader.
// 
// The class encapsulates the logic of setting up and managing the rendering of lines in the program.
//
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

#include "IPixelShader.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d3d11.lib")

class PixelShaderPolyLine : public IPixelShader
{
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

public:
	PixelShaderPolyLine();
	PixelShaderPolyLine(const PixelShaderPolyLine& other) = delete;
	PixelShaderPolyLine& operator=(const PixelShaderPolyLine& other) = delete;

	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>) override;
	void Shutdown() override;
	bool Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext>, int vertexCount, const Light& light) override;

	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPixelShader(void) override;

private:
	bool InitializeShader(Microsoft::WRL::ComPtr<ID3D11Device>, const WCHAR*);
	void ShutdownShader();

	void RenderShader(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, int vertexCount);
};
typedef std::shared_ptr<PixelShaderPolyLine> PixelShaderPolyLinePtr;
#endif // !SHADER_H
