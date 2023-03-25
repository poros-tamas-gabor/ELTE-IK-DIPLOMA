#ifndef PIXEL_SHADER_POLYLINE_H
#define	PIXEL_SHADER_POLYLINE_H


#include "IPixelShader.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d3d11.lib")

class PixelShaderPolyLine : public IPixelShader
{
private:
	ID3D11PixelShader* m_pixelShader;

public:
	PixelShaderPolyLine();
	PixelShaderPolyLine(const PixelShaderPolyLine& other) = delete;
	PixelShaderPolyLine& operator=(const PixelShaderPolyLine& other) = delete;

	bool Initialize(ID3D11Device*, HWND) override;
	void Shutdown() override;
	bool Render(ID3D11DeviceContext*, int vertexCount, const Light& light) override;

	ID3D11PixelShader* GetPixelShader(void) override;

private:
	bool InitializeShader(ID3D11Device*, HWND, const WCHAR*);
	void ShutdownShader();

	void RenderShader(ID3D11DeviceContext* deviceContext, int vertexCount);
};
#endif // !SHADER_H
