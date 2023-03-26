#include	"PolygonMesh.h"
#include	"../ErrorHandler.h"
#include	<memory>

bool PolygonMesh::Initialize(ID3D11Device* device, VertexMesh* vertices, UINT indexCount)
{
	if (device == nullptr)
		return false;

	bool bresult;
	bresult = this->InitializeBuffers(device, vertices, indexCount);
	if (!bresult)
	{
		return false;
	}
	return true;
}
void PolygonMesh::Shutdown()
{
	// Shutdown the vertex and index buffers.
	this->ShutdownBuffers();
}
void PolygonMesh::Render(ID3D11DeviceContext* deviceContext, IVertexShader& vertexShader, IPixelShader& pixelShader, DirectX::XMMATRIX worldMat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat,const Light& light)
{
	try {
		bool bresult = vertexShader.Render(deviceContext, worldMat, viewMat, projectionMat, light);
		if (!bresult)
			THROW_TREXCEPTION(L"Failed to render vertex shader");

		this->RenderBuffers(deviceContext);

		bresult = pixelShader.Render(deviceContext, this->GetVertexCount(), light);
		if(!bresult)
			THROW_TREXCEPTION(L"Failed to render pixel shader");


	}
	catch (TRException& exception)
	{
		ErrorHandler::Log(exception);
	}
}

int PolygonMesh::GetIndexCount() const
{
	return this->_indexCount;
}
int PolygonMesh::GetVertexCount() const
{
	return this->_vertexCount;

}

bool PolygonMesh::InitializeBuffers(ID3D11Device* device, VertexMesh* vertices, UINT indexCount)
{
	std::unique_ptr<unsigned long[]>		indices;
	D3D11_BUFFER_DESC						vertexBufferDesc, indexBufferDesc;
	HRESULT									result;
	D3D11_SUBRESOURCE_DATA					vertexData, indexData;

	try {
		// Set the number of vertices in the vertex array.
		// Set the number of indices in the index array.
		this->_indexCount = this->_vertexCount = indexCount;

		//Allocate memory for the array of vertices and indices
		indices = std::make_unique<unsigned long[]>(this->_vertexCount);

		for (unsigned int i = 0; i < indexCount; i++)
		{
			//vertices[i].color = { 1.0f, 0.0f, 0.0f, 1.0f };
			//vertices[i].normal = { v.normal.x , v.normal.y, v.normal.z };
			//vertices[i].position = { v.position.x/100,v.position.y/100,v.position.z/100};
			indices[i] = i;
			i++;
		}

		// Set up the description of the static vertex buffer.
		ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexMesh) * this->_vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);

		THROW_COM_EXCEPTION_IF_FAILED(result, L"Failed to Create vertexbuffer");

		// Set up the description of the static index buffer.
		ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
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

		THROW_COM_EXCEPTION_IF_FAILED(result, L"Failed to Create indexbuffer");
	}
	catch (const COMException& exception)
	{
		ErrorHandler::Log(exception);
		return false;
	}
	return true;

	
}
void PolygonMesh::ShutdownBuffers()
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
void PolygonMesh::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	//set vertex buffer stride and offset
	stride = sizeof(VertexMesh);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &this->m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}