#ifndef DEPTH_PIXEL_SHADER_H
#define	DEPTH_PIXEL_SHADER_H

#include "IPixelShader.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d3d11.lib")

class DepthPixelShader : public IPixelShader
{

private:
	ID3D11PixelShader* m_pixelShader;



public:
	DepthPixelShader();
	DepthPixelShader(const DepthPixelShader& other) = delete;
	DepthPixelShader& operator=(const DepthPixelShader& other) = delete;

	bool Initialize(ID3D11Device*, HWND) override;
	void Shutdown() override;
	bool Render(ID3D11DeviceContext*, int vertexCount, const Light& light) override;

	ID3D11PixelShader* GetPixelShader(void) override;

private:
	bool SetShadeParameters(ID3D11DeviceContext* deviceContext, const Light& light);
	bool InitializeShader(ID3D11Device*, HWND, const WCHAR*);
	void ShutdownShader();

	void RenderShader(ID3D11DeviceContext* deviceContext, int vertexCount);
};
#endif // !SHADER_H

