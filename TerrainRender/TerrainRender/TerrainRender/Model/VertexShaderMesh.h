#ifndef VERTEX_SHADER_MESH_H
#define	VERTEX_SHADER_MESH_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "Light.h"
#include "IVertexShader.h"
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d3d11.lib")

class VertexShaderMesh : public IVertexShader
{
public:
	struct MatrixBuffer
	{
		DirectX::XMMATRIX worldMat;
		DirectX::XMMATRIX viewMat;
		DirectX::XMMATRIX projectionMat;
		DirectX::XMFLOAT4 color;
	};

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;

public:
	VertexShaderMesh();
	VertexShaderMesh(const VertexShaderMesh& other) = delete;
	VertexShaderMesh& operator=(const VertexShaderMesh& other) = delete;


	bool Initialize(ID3D11Device*, HWND) override;
	void Shutdown() override;
	bool Render(ID3D11DeviceContext*, DirectX::XMMATRIX worldmat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat, DirectX::XMFLOAT4 color) override;

	ID3D11VertexShader* GetVertexShader(void) override;
	ID3D11InputLayout* GetInputLayout(void) override;

private:
	bool SetShadeParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldmat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat, DirectX::XMFLOAT4 color);
	bool InitializeShader(ID3D11Device*, HWND, const WCHAR*);
	void ShutdownShader();
	void RenderShader(ID3D11DeviceContext* deviceContext);
};

#endif // !SHADER_H
