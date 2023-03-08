#include "GfxModel.h"
#include	"../ErrorHandler.h"
#include "../Model/ModelLayer.h"

bool GfxModel::Initialize(ID3D11Device* device, const ModelLayer* _model)
{
	bool bresult;
	bresult = this->InitializeBuffers(device,_model);
	if (!bresult)
	{
		return false;
	}
	return true;
}
void GfxModel::Shutdown()
{
	this->ShutdownBuffers();
}
void GfxModel::Render(ID3D11DeviceContext* deviceContext)
{
	this->RenderBuffers(deviceContext);
}

int GfxModel::GetIndexCount()
{
	return this->m_indexCount;
}
int GfxModel::GetVertexCount()
{
	return this->m_vertexCount;

}

bool GfxModel::InitializeBuffers(ID3D11Device* device, const ModelLayer* _model)
{
	std::vector<ModelVertex> modelVertices;
	_model->GetVertices(modelVertices);

	std::unique_ptr<GfxVertex[]>			vertices;
	std::unique_ptr<unsigned long[]>		indices;
	D3D11_BUFFER_DESC					vertexBufferDesc, indexBufferDesc;
	HRESULT								result;
	D3D11_SUBRESOURCE_DATA				vertexData, indexData;


	this->m_indexCount = this->m_vertexCount = modelVertices.size();

	vertices = std::make_unique<GfxVertex[]>(this->m_indexCount);
	indices = std::make_unique<unsigned long[]>(this->m_vertexCount);

	int i = 0;
	for (const ModelVertex& v : modelVertices)
	{
		vertices[i].color = { 1.0f, 0.0f, 0.0f, 1.0f };
		vertices[i].position = { v.position.x / 100,v.position.y / 100,v.position.z / 100 };
		indices[i] = i;
		i++;

	}


	// Set up the description of the static vertex buffer.
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(GfxVertex) * this->m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices.get();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		ErrorHandler::log(result, L"Failed to Create vertexbuffer");
		return false;
	}

	// Set up the description of the static index buffer.
	ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices.get();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		ErrorHandler::log(result, L"Failed to Create indexbuffer");
		return false;
	}

	return true;

	
}
void GfxModel::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}
void GfxModel::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	//set vertex buffer stride and offset
	stride = sizeof(GfxVertex);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &this->m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}