#include "PixelShaderPolyLine.h"
#include "../ErrorHandler.h"

PixelShaderPolyLine::PixelShaderPolyLine() : m_pixelShader(nullptr) {}

bool PixelShaderPolyLine::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	bool result;

	result = this->InitializeShader(device, L"pixelShaderPolyLine.cso");
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

bool PixelShaderPolyLine::InitializeShader(Microsoft::WRL::ComPtr<ID3D11Device> device, const WCHAR* psFilename)
{
	HRESULT								result;
	Microsoft::WRL::ComPtr<ID3D10Blob>	pixelShaderBuffer = nullptr;
	UINT								flags = D3DCOMPILE_ENABLE_STRICTNESS;

	std::wstring shaderfolder = L"";
#pragma region DetermineShaderPath
	if (IsDebuggerPresent() == TRUE)
	{
#ifdef _DEBUG //Debug Mode
#ifdef _WIN64 //x64
		shaderfolder = L"..\\x64\\Debug\\";
#else  //x86 (Win32)
		shaderfolder = L"..\\Win32\\Debug\\";
#endif
#else //Release Mode
#ifdef _WIN64 //x64
		shaderfolder = L"..\\x64\\Release\\";
#else  //x86 (Win32)
		shaderfolder = L"..\\Win32\\Release\\";
#endif
#endif
	}
	try
	{
		// Compile the pixel shader code.
		result = D3DReadFileToBlob((shaderfolder + psFilename).c_str(), pixelShaderBuffer.ReleaseAndGetAddressOf());
		if (FAILED(result))
		{
			std::wstring errormsg = L"Failed to Compile the pixel shader code. Filename: ";
			errormsg += psFilename;
			THROW_COM_EXCEPTION_IF_FAILED(result, errormsg);
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

