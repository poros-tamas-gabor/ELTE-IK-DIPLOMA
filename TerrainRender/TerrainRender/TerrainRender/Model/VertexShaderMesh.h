#ifndef VERTEX_SHADER_MESH_H
#define	VERTEX_SHADER_MESH_H

///////////////////////////////////////////////////////////////////////////////
// VertexShaderMesh.h
// ==================
// 
// VertexShaderMesh is a class derived from the IVertexShader interface.
// Its main purpose is to set up the vertex shader of the meshes by 
// - compiling the vertex shader from the.hlsl file, 
// - allocating memory for the constant buffers, 
// - creating the DirectX vertex shader, 
// - and using the device context to render the vertex shader.
// 
// The class encapsulates the logic of setting up and managing the rendering of meshes in the program.
//
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "Light.h"
#include "IVertexShader.h"
#include <memory>
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
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_matrixBuffer;

public:
	VertexShaderMesh();
	VertexShaderMesh(const VertexShaderMesh& other) = delete;
	VertexShaderMesh& operator=(const VertexShaderMesh& other) = delete;


	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>) override;
	void Shutdown() override;
	bool Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext>, DirectX::XMMATRIX worldmat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat, DirectX::XMFLOAT4 color) override;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetVertexShader(void) override;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> GetInputLayout(void) override;

private:
	bool SetShaderParameters(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, DirectX::XMMATRIX worldmat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat, DirectX::XMFLOAT4 color);
	bool InitializeShader(Microsoft::WRL::ComPtr<ID3D11Device>, const WCHAR*);
	void ShutdownShader();
	void RenderShader(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext);
};

typedef std::shared_ptr<VertexShaderMesh> VertexShaderMeshPtr;

#endif // !SHADER_H
