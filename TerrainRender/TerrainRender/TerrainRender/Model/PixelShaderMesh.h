#ifndef PIXEL_SHADER_MESH_H
#define	PIXEL_SHADER_MESH_H


#include "IPixelShader.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d3d11.lib")

class PixelShaderMesh : public IPixelShader
{
public:
	struct LightBuffer
	{
		DirectX::XMFLOAT4	ambientColor;
		DirectX::XMFLOAT4	diffuseColor;
		DirectX::XMFLOAT4	inverseLightDirection;
	};

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_lightBuffer;

public:
	PixelShaderMesh();
	PixelShaderMesh(const PixelShaderMesh& other) = delete;
	PixelShaderMesh& operator=(const PixelShaderMesh& other) = delete;

	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>, HWND) override;
	void Shutdown() override;
	bool Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext>, int vertexCount, const Light& light) override;

	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPixelShader(void) override;

private:
	bool SetShadeParameters(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, const Light& light);
	bool InitializeShader(Microsoft::WRL::ComPtr<ID3D11Device>, HWND, const WCHAR*);
	void ShutdownShader();

	void RenderShader(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, int vertexCount);
};

typedef std::shared_ptr<PixelShaderMesh> PixelShaderMeshPtr;
#endif // !SHADER_H
