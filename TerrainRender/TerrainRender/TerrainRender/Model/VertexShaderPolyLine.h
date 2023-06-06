#ifndef VERTEX_SHADER_POLYLINE_H
#define	VERTEX_SHADER_POLYLINE_H

///////////////////////////////////////////////////////////////////////////////
// VertexShaderPolyline.h
// ==================
// 
// VertexShaderMesh is a class derived from the IVertexShader interface.
// Its main purpose is to set up the vertex shader of the lines by 
// - compiling the vertex shader from the.hlsl file, 
// - allocating memory for the constant buffers, 
// - creating the DirectX vertex shader, 
// - and using the device context to render the vertex shader.
// 
// The class encapsulates the logic of setting up and managing the rendering of lines in the program.
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
#include <string>
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d3d11.lib")

class VertexShaderPolyLine : public IVertexShader
{
public:
	struct MatrixBuffer
	{
		DirectX::XMMATRIX worldMat;
		DirectX::XMMATRIX viewMat;
		DirectX::XMMATRIX projectionMat;
	};

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_matrixBuffer;

public:
	VertexShaderPolyLine();
	VertexShaderPolyLine(const VertexShaderPolyLine& other) = delete;
	VertexShaderPolyLine& operator=(const VertexShaderPolyLine& other) = delete;


	virtual bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>) override;
	virtual void Shutdown() override;
	virtual bool Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext>, DirectX::XMMATRIX worldmat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat, DirectX::XMFLOAT4 color) override;

	virtual Microsoft::WRL::ComPtr<ID3D11VertexShader> GetVertexShader(void) override;
	virtual Microsoft::WRL::ComPtr<ID3D11InputLayout> GetInputLayout(void) override;

private:
	bool SetShaderParameters(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, DirectX::XMMATRIX worldmat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat);
	bool InitializeShader(Microsoft::WRL::ComPtr<ID3D11Device>, const std::wstring&);
	void ShutdownShader();
	void RenderShader(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext);
};

typedef std::shared_ptr<VertexShaderPolyLine> VertexShaderPolyLinePtr;

#endif // !VERTEX_SHADER_GRID_H
