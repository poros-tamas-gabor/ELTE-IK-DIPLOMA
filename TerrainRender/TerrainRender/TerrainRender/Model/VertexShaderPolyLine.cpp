#include "VertexShaderPolyLine.h"
#include "../ErrorHandler.h"
#include <d3dcompiler.h>
#pragma comment(lib, "D3DCompiler.lib")

VertexShaderPolyLine::VertexShaderPolyLine() : m_vertexShader(nullptr), m_layout(nullptr), m_matrixBuffer(nullptr) {}

bool VertexShaderPolyLine::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	bool result;

	result = this->InitializeShader(device, L"vertexShaderPolyLine.cso");
	if (!result)
	{
		return false;
	}
	return true;
}

void VertexShaderPolyLine::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	this->ShutdownShader();
	return;
}

void VertexShaderPolyLine::ShutdownShader()
{
	if (this->m_vertexShader)
	{
		this->m_vertexShader->Release();
		this->m_vertexShader = nullptr;
	}
	if (this->m_layout)
	{
		this->m_layout->Release();
		this->m_layout = nullptr;
	}
	if (this->m_matrixBuffer)
	{
		this->m_matrixBuffer->Release();
		this->m_matrixBuffer = nullptr;
	}
}

bool VertexShaderPolyLine::InitializeShader(Microsoft::WRL::ComPtr<ID3D11Device> device, const std::wstring& vsFilename)
{
	HRESULT									result;
	Microsoft::WRL::ComPtr<ID3D10Blob> 		vertexShaderBuffer = nullptr;
	D3D11_INPUT_ELEMENT_DESC				polygonLayout[2];
	unsigned int							numElements;
	D3D11_BUFFER_DESC						matrixBufferDesc;

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
		result = D3DReadFileToBlob( (shaderfolder + vsFilename).c_str(), vertexShaderBuffer.ReleaseAndGetAddressOf());
		//result = D3DCompileFromFile(vsFilename, NULL, NULL, "main", "vs_5_0", flags, 0, &vertexShaderBuffer, &errorMessage);
		if (FAILED(result))
		{
			std::wstring errormsg = L"Failed to Compile the vertex shader code. Filename: ";
			errormsg += vsFilename;
			THROW_COM_EXCEPTION_IF_FAILED(result, errormsg);
		}
		// Create the vertex shader from the buffer.
		result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, this->m_vertexShader.ReleaseAndGetAddressOf());
		
		THROW_COM_EXCEPTION_IF_FAILED(result, L"Failed to Create the vertex shader from the buffer");

		//Create vertx input description
		// This setup needs to match the VertexType stucture in the IRenderable and in the shader.
		polygonLayout[0].SemanticName = "POSITION";
		polygonLayout[0].SemanticIndex = 0;
		polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polygonLayout[0].InputSlot = 0;
		polygonLayout[0].AlignedByteOffset = 0;
		polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[0].InstanceDataStepRate = 0;

		polygonLayout[1].SemanticName = "COLOR";
		polygonLayout[1].SemanticIndex = 0;
		polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		polygonLayout[1].InputSlot = 0;
		polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polygonLayout[1].InstanceDataStepRate = 0;

		numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

		// Create the vertex input layout.
		result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), m_layout.ReleaseAndGetAddressOf());

		THROW_COM_EXCEPTION_IF_FAILED(result, L"Failed to Create the vertex input layout");

		vertexShaderBuffer->Release();
		vertexShaderBuffer = nullptr;

		// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		matrixBufferDesc.ByteWidth = sizeof(VertexShaderPolyLine::MatrixBuffer);
		matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		matrixBufferDesc.MiscFlags = 0;
		matrixBufferDesc.StructureByteStride = 0;

		// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
		result = device->CreateBuffer(&matrixBufferDesc, NULL, m_matrixBuffer.ReleaseAndGetAddressOf());

		THROW_COM_EXCEPTION_IF_FAILED(result, L"Failed to Create constant buffer");
		return true;
	}
	catch (const COMException &exception)
	{
		ErrorHandler::Log(exception);
	}


	return false;

}

void VertexShaderPolyLine::RenderShader(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext)
{
	deviceContext->IASetInputLayout(this->m_layout.Get());

	deviceContext->VSSetShader(this->m_vertexShader.Get(), NULL, 0);
}
bool VertexShaderPolyLine::Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, DirectX::XMMATRIX worldMat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat, DirectX::XMFLOAT4 color)
{
	bool bresult;
	bresult = this->SetShaderParameters(deviceContext, worldMat, viewMat, projectionMat);
	if (!bresult)
	{
		return false;
	}
	this->RenderShader(deviceContext);
	return true;
}

bool VertexShaderPolyLine::SetShaderParameters(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, DirectX::XMMATRIX worldMat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat)
{
	HRESULT						result;
	D3D11_MAPPED_SUBRESOURCE	mappedResource;
	MatrixBuffer*				matrixDataPtr;
	unsigned int				bufferNumber;

	try {
		// Transpose the matrices to prepare them for the shader.
		worldMat = DirectX::XMMatrixTranspose(worldMat);
		viewMat = DirectX::XMMatrixTranspose(viewMat);
		projectionMat = DirectX::XMMatrixTranspose(projectionMat);

		// Lock the constant buffer so it can be written to.
		result = deviceContext->Map(m_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		THROW_COM_EXCEPTION_IF_FAILED(result, L"Failed to lock the constant buffer");

		// Get a pointer to the data in the constant buffer.
		matrixDataPtr = static_cast<MatrixBuffer*> (mappedResource.pData);

		// Copy the data into the constant buffer.
		matrixDataPtr->worldMat = worldMat;
		matrixDataPtr->viewMat = viewMat;
		matrixDataPtr->projectionMat = projectionMat;

		// Unlock the constant buffer.
		deviceContext->Unmap(m_matrixBuffer.Get(), 0);

		// Set the position of the constant buffer in the vertex shader.
		bufferNumber = 0;

		// Finanly set the constant buffer in the vertex shader with the updated values.
		deviceContext->VSSetConstantBuffers(bufferNumber, 1, m_matrixBuffer.GetAddressOf());

		mappedResource = { 0 };
	}
	catch (const COMException& exception)
	{
		ErrorHandler::Log(exception);
		return false;
	}

	return true;
}

Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShaderPolyLine::GetVertexShader(void)
{
	return this->m_vertexShader;
}
Microsoft::WRL::ComPtr<ID3D11InputLayout> VertexShaderPolyLine::GetInputLayout(void)
{
	return this->m_layout;
}