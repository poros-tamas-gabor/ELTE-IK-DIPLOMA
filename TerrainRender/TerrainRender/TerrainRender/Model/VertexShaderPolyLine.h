#ifndef VERTEX_SHADER_POLYLINE_H
#define	VERTEX_SHADER_POLYLINE_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "Light.h"
#include "IVertexShader.h"
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
	ID3D11VertexShader* m_vertexShader;
	ID3D11InputLayout*	m_layout;
	ID3D11Buffer*		m_matrixBuffer;

public:
	VertexShaderPolyLine();
	VertexShaderPolyLine(const VertexShaderPolyLine& other) = delete;
	VertexShaderPolyLine& operator=(const VertexShaderPolyLine& other) = delete;


	virtual bool Initialize(ID3D11Device*, HWND) override;
	virtual void Shutdown() override;
	virtual bool Render(ID3D11DeviceContext*, DirectX::XMMATRIX worldmat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat, const Light&) override;

	virtual ID3D11VertexShader* GetVertexShader(void) override;
	virtual ID3D11InputLayout* GetInputLayout(void) override;

private:
	bool SetShadeParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldmat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat);
	bool InitializeShader(ID3D11Device*, HWND, const WCHAR*);
	void ShutdownShader();
	void RenderShader(ID3D11DeviceContext* deviceContext);
};

#endif // !VERTEX_SHADER_GRID_H