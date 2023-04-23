#include "PixelShaderMesh.h"
#include "../ErrorHandler.h"

PixelShaderMesh::PixelShaderMesh() : m_pixelShader(nullptr), m_lightBuffer(nullptr) {}

bool PixelShaderMesh::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, HWND hwnd)
{
	bool result;

	result = this->InitializeShader(device, hwnd, L"pixelShaderMesh.cso");
	if (!result)
	{
		return false;
	}
	return true;
}

void PixelShaderMesh::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	this->ShutdownShader();
	return;
}

void PixelShaderMesh::ShutdownShader()
{
	//if (this->m_pixelShader)
	//{
	//	this->m_pixelShader->Release();
	//	this->m_pixelShader = nullptr;
	//}
	//if (this->m_lightBuffer)
	//{
	//	this->m_lightBuffer->Release();
	//	this->m_lightBuffer = nullptr;
	//}
}

bool PixelShaderMesh::InitializeShader(Microsoft::WRL::ComPtr<ID3D11Device> device, HWND hwnd, const std::wstring& psFilename)
{
	HRESULT								result;
	Microsoft::WRL::ComPtr<ID3D10Blob>	pixelShaderBuffer = nullptr;
	D3D11_BUFFER_DESC					lightBufferDesc = {0};
	UINT								flags = D3DCOMPILE_ENABLE_STRICTNESS;

	std::wstring shaderfolder = L"";
#pragma region DetermineShaderPath
	if (IsDebuggerPresent() == TRUE)
	{
#ifdef _DEBUG //Debug Mode
#ifdef _WIN64 //x64
		shaderfolder = L"..\\x64\\Debug\\";
#else  //x86 (Win32)
		shaderfolder = L"..\\Debug\\";
#endif
#else //Release Mode
#ifdef _WIN64 //x64
		shaderfolder = L"..\\x64\\Release\\";
#else  //x86 (Win32)
		shaderfolder = L"..\\Release\\";
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


		//setup light
		// Setup the description of the dynamic matrix constant buffer that is in the pixel shader.
		lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		lightBufferDesc.ByteWidth = sizeof(LightBuffer);
		lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		lightBufferDesc.MiscFlags = 0;
		lightBufferDesc.StructureByteStride = 0;
		
		// Create the constant buffer pointer so we can access the pixel shader constant buffer from within this class.
		result = device->CreateBuffer(&lightBufferDesc, NULL, m_lightBuffer.ReleaseAndGetAddressOf());
		
		THROW_COM_EXCEPTION_IF_FAILED(result, L"Failed to Create constant buffer for light");
	}

	catch (const COMException& exception)
	{
		ErrorHandler::Log(exception);
		return false;
	}


	return true;

}

Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShaderMesh::GetPixelShader(void)
{
	return this->m_pixelShader;
}

bool PixelShaderMesh::Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, int vertexCount, const Light& light)
{
	bool result = true;
	result = this->SetShadeParameters(deviceContext, light);
	this->RenderShader(deviceContext, vertexCount);
	return result;
}
void PixelShaderMesh::RenderShader(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, int indexCount) {

	deviceContext->PSSetShader(this->m_pixelShader.Get(), NULL, 0);
	deviceContext->DrawIndexed(indexCount, 0, 0);
}

bool PixelShaderMesh::SetShadeParameters(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, const Light& light)
{
	HRESULT						result;
	D3D11_MAPPED_SUBRESOURCE	mappedResource;
	LightBuffer*				lightDataPtr = NULL;
	unsigned int				bufferNumber;

	try {
		// Set the position of the constant buffer in the pixel shader.
		bufferNumber = 0;

		mappedResource = { 0 };
		// Lock the constant buffer so it can be written to.
		result = deviceContext->Map(m_lightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		THROW_COM_EXCEPTION_IF_FAILED(result, L"Failed to lock the constant buffer");

		lightDataPtr = static_cast<LightBuffer*>(mappedResource.pData);

		lightDataPtr->ambientColor = light.GetAmbientColor();
		lightDataPtr->diffuseColor = light.GetDiffuseColor();
		lightDataPtr->inverseLightDirection = light.GetInverseDirection();
		// Unlock the constant buffer.
		deviceContext->Unmap(this->m_lightBuffer.Get(), 0);
		// Finanly set the constant buffer in the pixel shader with the updated values.
		deviceContext->PSSetConstantBuffers(bufferNumber, 1, m_lightBuffer.GetAddressOf());
	}
	catch (const COMException& exception)
	{
		ErrorHandler::Log(exception);
		return false;
	}


	return true;
}

