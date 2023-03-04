#ifndef SHADER_H
#define	SHADER_H

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d3d11.lib")

class  Shader
{
public:
	virtual bool Initialize(ID3D11Device*, HWND) = 0;
	virtual void Shutdown() = 0;

protected:
	virtual bool InitializeShader(ID3D11Device*, HWND, const WCHAR*) = 0;
	virtual void ShutdownShader() = 0;
};

class VertexShader : Shader
{

private:
	ID3D11VertexShader*		m_vertexShader;
	ID3D11InputLayout*		m_layout;
	ID3D11Buffer*			m_matrixBuffer;

public:
	VertexShader();
	VertexShader(const VertexShader& other) = delete;
	VertexShader& operator=(const VertexShader& other) = delete;


	bool Initialize(ID3D11Device*, HWND) override;
	void Shutdown() override;
	bool Render(ID3D11DeviceContext*);

	ID3D11VertexShader* GetVertexShader(void);
	ID3D11InputLayout*	GetInputLayout(void);


private:
	bool InitializeShader(ID3D11Device*, HWND,const WCHAR*) override;
	void ShutdownShader() override;
	void RenderShader(ID3D11DeviceContext* deviceContext);

	//void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	//bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	//void RenderShader(ID3D11DeviceContext*, int);
};

class PixelShader : Shader
{

private:
	ID3D11PixelShader*		m_pixelShader;
	ID3D11Buffer*			m_matrixBuffer;

public:
	PixelShader();
	PixelShader(const PixelShader& other) = delete;
	PixelShader& operator=(const PixelShader& other) = delete;


	bool Initialize(ID3D11Device*, HWND) override;
	void Shutdown() override;
	bool Render(ID3D11DeviceContext*);


	//bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

	ID3D11PixelShader* GetPixelShader(void);

private:
	bool InitializeShader(ID3D11Device*, HWND, const WCHAR*) override;
	void ShutdownShader() override;
	//void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	//bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);
};
#endif // !SHADER_H
