#include "LineList.h"

#include	"../ErrorHandler.h"
#include	<memory>

bool LineList::Initialize(ID3D11Device* device, IVertexShader* vertexShader, IPixelShader* pixelShader, VertexPolygon* vertices, UINT indexCount)
{
	if (device == nullptr || vertexShader == nullptr || pixelShader == nullptr)
		return false;

	this->m_pixelShader = pixelShader;
	this->m_vertexShader = vertexShader;

	bool bresult;
	bresult = this->InitializeBuffers(device, vertices, indexCount);
	if (!bresult)
	{
		return false;
	}
	return true;
}
void LineList::Shutdown()
{
	// Shutdown the vertex and index buffers.
	this->ShutdownBuffers();
}
void LineList::Render(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat, const Light& light)
{
	bool bresult = this->m_vertexShader->Render(deviceContext, worldMat, viewMat, projectionMat, light);
	if (!bresult)
	{
		//return false;
	}
	this->RenderBuffers(deviceContext);
	this->m_pixelShader->Render(deviceContext, this->GetVertexCount());

}

int LineList::GetIndexCount() const
{
	return 0;
}
int LineList::GetVertexCount() const
{
	return this->_vertexCount;

}

bool LineList::InitializeBuffers(ID3D11Device* device, VertexPolygon* vertices, UINT indexCount)
{

	D3D11_BUFFER_DESC						vertexBufferDesc;
	HRESULT									result;
	D3D11_SUBRESOURCE_DATA					vertexData, indexData;

	// Set the number of vertices in the vertex array.
	// Set the number of indices in the index array.
	this->_vertexCount = indexCount;

	try
	{
		// Set up the description of the static vertex buffer.
		ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexPolygon) * this->_vertexCount;
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
	}
	catch (const COMException& exception)
	{
		ErrorHandler::Log(exception);
		return false;
	}
	return true;


}
void LineList::ShutdownBuffers()
{

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}
void LineList::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	//set vertex buffer stride and offset
	stride = sizeof(VertexPolygon);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &this->m_vertexBuffer, &stride, &offset);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}