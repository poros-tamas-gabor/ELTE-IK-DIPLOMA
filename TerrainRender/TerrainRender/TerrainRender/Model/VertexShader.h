#ifndef VERTEX_SHADER_H
#define	VERTEX_SHADER_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "Light.h"
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d3d11.lib")

class VertexShader
{
public:
	struct MatrixBuffer
	{
		DirectX::XMMATRIX worldMat;
		DirectX::XMMATRIX viewMat;
		DirectX::XMMATRIX projectionMat;
	};

	struct LightBuffer
	{
		DirectX::XMFLOAT4	ambientColor;
		DirectX::XMFLOAT4	diffuseColor;
		DirectX::XMFLOAT4	lightDirection;
	};

private:
	ID3D11VertexShader*		m_vertexShader;
	ID3D11InputLayout*		m_layout;
	ID3D11Buffer*			m_matrixBuffer;
	ID3D11Buffer*			m_lightBuffer;

public:
	VertexShader();
	VertexShader(const VertexShader& other) = delete;
	VertexShader& operator=(const VertexShader& other) = delete;


	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, DirectX::XMMATRIX worldmat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat,const Light& light);

	ID3D11VertexShader* GetVertexShader(void);
	ID3D11InputLayout* GetInputLayout(void);

private:
	bool SetShadeParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldmat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat, DirectX::XMFLOAT4 ambientColor, DirectX::XMFLOAT4 diffuseColor, DirectX::XMFLOAT4	lightDirection);
	bool InitializeShader(ID3D11Device*, HWND, const WCHAR*);
	void ShutdownShader();
	void RenderShader(ID3D11DeviceContext* deviceContext);
};

#endif // !SHADER_H
