#include "VertexShader.h"
#include "../ErrorHandler.h"

VertexShader::VertexShader() : m_vertexShader(nullptr), m_layout(nullptr), m_matrixBuffer(nullptr) {}

bool VertexShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = this->InitializeShader(device, hwnd, L"Model/vertexShader.hlsl");
	if (!result)
	{
		return false;
	}
	return true;
}

void VertexShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	this->ShutdownShader();
	return;
}

void VertexShader::ShutdownShader()
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
	if (this->m_lightBuffer)
	{
		this->m_lightBuffer->Release();
		this->m_lightBuffer = nullptr;
	}
}

bool VertexShader::InitializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* vsFilename)
{
	HRESULT						result;
	ID3D10Blob*					errorMessage = nullptr;
	ID3D10Blob*					vertexShaderBuffer = nullptr;
	D3D11_INPUT_ELEMENT_DESC	polygonLayout[3];
	unsigned int				numElements;
	D3D11_BUFFER_DESC			matrixBufferDesc;
	D3D11_BUFFER_DESC			lightBufferDesc;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif


	result = D3DCompileFromFile(vsFilename, NULL, NULL, "main", "vs_5_0", flags, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)  
		{
			std::wstring errormsg = L"Failed to Compile the vertex shader code. Filename: ";
			errormsg += vsFilename;
			ErrorHandler::log(result, errormsg);
		}
		else
		{
			ErrorHandler::log(result, L"Missing Shader File");
		}
		return false;
	}
	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &this->m_vertexShader);
	if (FAILED(result))
	{
		ErrorHandler::log(result, L"Failed to Create the vertex shader from the buffer");
		return false;
	}

	//Create vertx input description
	// This setup needs to match the VertexType stucture in the IRenderable and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "NORMAL";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "COLOR";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),&m_layout);

	if (FAILED(result))
	{
		ErrorHandler::log(result, L"Failed to Create the vertex input layout");
		return false;
	}

	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(VertexShader::MatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		ErrorHandler::log(result, L"Failed to Create constant buffer");
		return false;
	}

	lightBufferDesc = { 0 };
	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(VertexShader::LightBuffer);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if (FAILED(result))
	{
		ErrorHandler::log(result, L"Failed to Create constant buffer");
		return false;
	}

	return true;

}

void VertexShader::RenderShader(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetInputLayout(this->m_layout);

	deviceContext->VSSetShader(this->m_vertexShader, NULL, 0);
}
bool VertexShader::Render(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat,const Light& light)
{
	bool bresult;
	bresult = this->SetShadeParameters(deviceContext, worldMat, viewMat, projectionMat,light.GetAmbientColor(), light.GetDiffuseColor(), light.GetInverseDirection());
	if(!bresult)
	{
		return false;
	}
	this->RenderShader(deviceContext);
	return true;
}

bool VertexShader::SetShadeParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat, DirectX::XMFLOAT4	ambientColor, DirectX::XMFLOAT4 diffuseColor, DirectX::XMFLOAT4	lightDirection)
{
	HRESULT						result;
	D3D11_MAPPED_SUBRESOURCE	mappedResource;
	MatrixBuffer*				matrixDataPtr;
	LightBuffer*				lightDataPtr;

	unsigned int				bufferNumber;

	// Transpose the matrices to prepare them for the shader.
	worldMat = DirectX::XMMatrixTranspose(worldMat);
	viewMat = DirectX::XMMatrixTranspose(viewMat);
	projectionMat = DirectX::XMMatrixTranspose(projectionMat);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		ErrorHandler::log(result, L"Failed to lock the constant buffer");
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	matrixDataPtr = static_cast<MatrixBuffer*> (mappedResource.pData);

	// Copy the data into the constant buffer.
	matrixDataPtr->worldMat = worldMat;
	matrixDataPtr->viewMat = viewMat;
	matrixDataPtr->projectionMat = projectionMat;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);


	mappedResource = { 0 };
	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		ErrorHandler::log(result, L"Failed to lock the constant buffer");
		return false;
	}

	lightDataPtr = static_cast<LightBuffer*>(mappedResource.pData);

	lightDataPtr->ambientColor = ambientColor;
	lightDataPtr->diffuseColor = diffuseColor;
	lightDataPtr->lightDirection = lightDirection;

	deviceContext->Unmap(this->m_lightBuffer, 0);

	bufferNumber = 1;
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);



	return true;
}

ID3D11VertexShader* VertexShader::GetVertexShader(void)
{
	return this->m_vertexShader;
}
ID3D11InputLayout* VertexShader::GetInputLayout(void)
{
	return this->m_layout;
}