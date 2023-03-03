#include "Shader.h"

#include "Shader.h"
#include "../ErrorHandler.h"

PixelShader::PixelShader() : m_pixelShader(nullptr), m_matrixBuffer(nullptr) {}

bool PixelShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = this->InitializeShader(device, hwnd, L"Graphics/PixelShader.hlsl");
	if (!result)
	{
		return false;
	}
	return true;
}

void PixelShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	this->ShutdownShader();
	return;
}

void PixelShader::ShutdownShader()
{
	if (this->m_pixelShader)
	{
		this->m_pixelShader->Release();
		this->m_pixelShader = nullptr;
	}
	if (this->m_matrixBuffer)
	{
		this->m_matrixBuffer->Release();
		this->m_matrixBuffer = nullptr;
	}
}

bool PixelShader::InitializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* psFilename)
{
	HRESULT						result;
	ID3D10Blob*					errorMessage = nullptr;
	ID3D10Blob*					pixelShaderBuffer = nullptr;
	unsigned int				numElements;
	D3D11_BUFFER_DESC			matrixBufferDesc;


	// Compile the pixel shader code.
	result = D3DCompileFromFile(psFilename, NULL, NULL, "main", "ps_5_0", D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			std::wstring errormsg = L"Failed to Compile the pixel shader code. Filename: ";
			errormsg += psFilename;
			ErrorHandler::log(result, errormsg);
		}
		else
		{
			ErrorHandler::log(result, L"Missing Shader File");

		}
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &this->m_pixelShader);
	if (FAILED(result))
	{
		ErrorHandler::log(result, L"Failed to Create the pixel shader from the buffer");
	}

	return true;

}

ID3D11PixelShader* PixelShader::GetPixelShader(void)
{
	return this->m_pixelShader;
}

bool PixelShader::Render(ID3D11DeviceContext* deviceContext)
{
	this->RenderShader(deviceContext, 0);
	return true;
}
void PixelShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount) {

	deviceContext->PSSetShader(this->m_pixelShader, NULL, 0);

	deviceContext->Draw(3, 0);
}

