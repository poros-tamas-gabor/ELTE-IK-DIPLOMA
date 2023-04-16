#include "PixelShaderPolyLine.h"
#include "../ErrorHandler.h"

PixelShaderPolyLine::PixelShaderPolyLine() : m_pixelShader(nullptr) {}

bool PixelShaderPolyLine::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, HWND hwnd)
{
	bool result;

	result = this->InitializeShader(device, hwnd, L"Model/pixelShaderPolyLine.hlsl");
	if (!result)
	{
		return false;
	}
	return true;
}

void PixelShaderPolyLine::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	this->ShutdownShader();
	return;
}

void PixelShaderPolyLine::ShutdownShader()
{
	if (this->m_pixelShader)
	{
		this->m_pixelShader->Release();
		this->m_pixelShader = nullptr;
	}
}

bool PixelShaderPolyLine::InitializeShader(Microsoft::WRL::ComPtr<ID3D11Device> device, HWND hwnd, const WCHAR* psFilename)
{
	HRESULT						result;
	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	unsigned int				numElements;
	D3D11_BUFFER_DESC			lightBufferDesc;
	UINT						flags = D3DCOMPILE_ENABLE_STRICTNESS;
	try
	{
		// Compile the pixel shader code.
		result = D3DCompileFromFile(psFilename, NULL, NULL, "main", "ps_5_0", flags, 0, &pixelShaderBuffer, &errorMessage);
		if (FAILED(result))
		{
			if (errorMessage)
			{
				std::wstring errormsg = L"Failed to Compile the pixel shader code. Filename: ";
				errormsg += psFilename;
				throw COMException(result, errormsg, __FILEW__, __FUNCTIONW__, __LINE__);
			}
			else
				throw COMException(result, L"Missing Shader File", __FILEW__, __FUNCTIONW__, __LINE__);
		}

		// Create the pixel shader from the buffer.
		result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, this->m_pixelShader.ReleaseAndGetAddressOf());

		THROW_COM_EXCEPTION_IF_FAILED(result, L"Failed to Create the pixel shader from the buffer");

		pixelShaderBuffer->Release();
		pixelShaderBuffer = nullptr;
	}

	catch (const COMException& exception)
	{
		ErrorHandler::Log(exception);
		return false;
	}


	return true;

}

Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShaderPolyLine::GetPixelShader(void)
{
	return this->m_pixelShader;
}

bool PixelShaderPolyLine::Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, int vertexCount, const Light& light)
{
	this->RenderShader(deviceContext, vertexCount);
	return true;
}
void PixelShaderPolyLine::RenderShader(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, int vertexCount) {

	deviceContext->PSSetShader(this->m_pixelShader.Get(), NULL, 0);
	deviceContext->Draw(vertexCount, 0);
}

