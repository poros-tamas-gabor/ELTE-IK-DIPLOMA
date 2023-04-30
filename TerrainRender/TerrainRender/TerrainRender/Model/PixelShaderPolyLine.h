#ifndef PIXEL_SHADER_POLYLINE_H
#define	PIXEL_SHADER_POLYLINE_H


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
