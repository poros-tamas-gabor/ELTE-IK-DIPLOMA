#ifndef PIXEL_SHADER_H
#define	PIXEL_SHADER_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d3d11.lib")

class PixelShader 
{

private:
	ID3D11PixelShader*		m_pixelShader;

public:
	PixelShader();
	PixelShader(const PixelShader& other) = delete;
	PixelShader& operator=(const PixelShader& other) = delete;

	bool Initialize(ID3D11Device*, HWND) ;
	void Shutdown() ;
	bool Render(ID3D11DeviceContext*, int vertexCount);

	ID3D11PixelShader* GetPixelShader(void);

private:
	bool InitializeShader(ID3D11Device*, HWND, const WCHAR*) ;
	void ShutdownShader() ;

	void RenderShader(ID3D11DeviceContext* deviceContext, int vertexCount);
};
#endif // !SHADER_H
