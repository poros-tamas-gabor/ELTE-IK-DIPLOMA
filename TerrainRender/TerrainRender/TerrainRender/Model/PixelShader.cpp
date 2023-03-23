#include "PixelShader.h"
#include "../ErrorHandler.h"

PixelShader::PixelShader() : m_pixelShader(nullptr) {}

bool PixelShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = this->InitializeShader(device, hwnd, L"Model/PixelShader.hlsl");
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
}

bool PixelShader::InitializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* psFilename)
{
	HRESULT						result;
	ID3D10Blob*					errorMessage = nullptr;
	ID3D10Blob*					pixelShaderBuffer = nullptr;
	unsigned int				numElements;
	D3D11_BUFFER_DESC			matrixBufferDesc;


	// Compile the pixel shader code.
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif

	try
	{
		result = D3DCompileFromFile(psFilename, NULL, NULL, "main", "ps_5_0", flags, 0, &pixelShaderBuffer, &errorMessage);
		if (FAILED(result))
		{
			if (errorMessage)
			{
				std::wstring errormsg = L"Failed to Compile the vertex shader code. Filename: ";
				errormsg += psFilename;
				throw COMException(result, errormsg, __FILEW__, __FUNCTIONW__, __LINE__);
			}
			else
				throw COMException(result, L"Missing Shader File", __FILEW__, __FUNCTIONW__, __LINE__);
		}

		// Create the pixel shader from the buffer.
		result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &this->m_pixelShader);

		THROW_COM_EXCEPTION_IF_FAILED(result, L"Failed to Create the pixel shader from the buffer");

		pixelShaderBuffer->Release();
	}

	catch (const COMException& exception)
	{
		ErrorHandler::Log(exception);
		return false;
	}


	return true;

}

ID3D11PixelShader* PixelShader::GetPixelShader(void)
{
	return this->m_pixelShader;
}

bool PixelShader::Render(ID3D11DeviceContext* deviceContext, int vertexCount)
{
	this->RenderShader(deviceContext, vertexCount);
	return true;
}
void PixelShader::RenderShader(ID3D11DeviceContext* deviceContext, int vertexCount) {

	deviceContext->PSSetShader(this->m_pixelShader, NULL, 0);
	deviceContext->Draw(vertexCount, 0);
}

