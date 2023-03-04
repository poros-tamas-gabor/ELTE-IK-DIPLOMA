#include "Shader.h"
#include "../ErrorHandler.h"

VertexShader::VertexShader() : m_vertexShader(nullptr), m_layout(nullptr), m_matrixBuffer(nullptr) {}

bool VertexShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = this->InitializeShader(device, hwnd, L"Graphics/vertexShader.hlsl");
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
}

bool VertexShader::InitializeShader(ID3D11Device* device, HWND hwnd, const WCHAR* vsFilename)
{
	HRESULT						result;
	ID3D10Blob*					errorMessage = nullptr;
	ID3D10Blob*					vertexShaderBuffer = nullptr;
	D3D11_INPUT_ELEMENT_DESC	polygonLayout[2];
	unsigned int				numElements;
	D3D11_BUFFER_DESC			matrixBufferDesc;

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

	//polygonLayout[1].SemanticName = "NORMAL";
	//polygonLayout[1].SemanticIndex = 0;
	//polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	//polygonLayout[1].InputSlot = 0;
	//polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	//polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	//polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

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
	return true;

}

void VertexShader::RenderShader(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetInputLayout(this->m_layout);

	deviceContext->VSSetShader(this->m_vertexShader, NULL, 0);
}
bool VertexShader::Render(ID3D11DeviceContext* deviceContext)
{
	this->RenderShader(deviceContext);
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