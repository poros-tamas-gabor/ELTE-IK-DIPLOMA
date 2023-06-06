#include	"PolygonMesh.h"
#include	"../ErrorHandler.h"
#include	<memory>


bool PolygonMesh::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, IVertexShaderPtr vertexShader, IPixelShaderPtr pixelShader, VertexMesh* vertices, unsigned long* indices, UINT vertexCount, UINT indexCount)
{
	bool bresult;

	if (device == nullptr || vertexShader == nullptr || pixelShader == nullptr || vertices == nullptr || indices == nullptr || vertexCount == 0 || indexCount == 0)
		return false;

	m_pixelShader	= pixelShader;
	m_vertexShader	= vertexShader;

	bresult = InitializeBuffers(device, vertices, indices, vertexCount, indexCount);

	ResetTransformation();

	return bresult;
}
void PolygonMesh::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();
}
void PolygonMesh::Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, DirectX::XMMATRIX worldMat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat,const Light& light)
{
	try {

		if (IsSeen())
		{
			bool bresult;

			m_worldMatrix = m_localMatrix * worldMat;

			bresult = m_vertexShader->Render(deviceContext, m_worldMatrix, viewMat, projectionMat, m_color.ToXMFLOAT4());
			THROW_TREXCEPTION_IF_FAILED(bresult, L"Failed to render vertex shader");

			RenderBuffers(deviceContext);

			bresult = m_pixelShader->Render(deviceContext, GetIndexCount(), light);
			THROW_TREXCEPTION_IF_FAILED(bresult, L"Failed to render pixel shader");
		}
	}
	catch (const COMException& e)
	{
		ErrorHandler::Log(e);
	}
	catch (const TRException& e)
	{
		ErrorHandler::Log(e);
	}
	catch (const std::exception& e)
	{
		ErrorHandler::Log(e);
	}
	catch (...)
	{
		ErrorHandler::Log("Unknown Exception: No details available");
	}
}

int PolygonMesh::GetIndexCount() const
{
	return m_indexCount;
}
int PolygonMesh::GetVertexCount() const
{
	return m_vertexCount;

}

bool PolygonMesh::InitializeBuffers(Microsoft::WRL::ComPtr<ID3D11Device> device, VertexMesh* vertices, unsigned long* indices, UINT vertexCount, UINT indexCount)
{
	D3D11_BUFFER_DESC						vertexBufferDesc, indexBufferDesc;
	HRESULT									result;
	D3D11_SUBRESOURCE_DATA					vertexData, indexData;

	try {
		// Set the number of vertices in the vertex array.
		// Set the number of indices in the index array.
		m_indexCount = indexCount;
		m_vertexCount = vertexCount;

		// Set up the description of the static vertex buffer.
		ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexMesh) * m_vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, m_vertexBuffer.ReleaseAndGetAddressOf());

		THROW_COM_EXCEPTION_IF_FAILED(result, L"Failed to Create vertexbuffer");

		// Set up the description of the static index buffer.
		ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Create the index buffer.
		result = device->CreateBuffer(&indexBufferDesc, &indexData, m_indexBuffer.ReleaseAndGetAddressOf());

		THROW_COM_EXCEPTION_IF_FAILED(result, L"Failed to Create indexbuffer");
		return true;
	}
	catch (const COMException& e)
	{
		ErrorHandler::Log(e);
	}
	catch (const TRException& e)
	{
		ErrorHandler::Log(e);
	}
	catch (const std::exception& e)
	{
		ErrorHandler::Log(e);
	}
	catch (...)
	{
		ErrorHandler::Log("Unknown Exception: No details available");
	}
	return false;
}
void PolygonMesh::ShutdownBuffers()
{
	// Release the index buffer.
	// Release the vertex buffer.
	return;
}
void PolygonMesh::RenderBuffers(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	//set vertex buffer stride and offset
	stride = sizeof(VertexMesh);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void PolygonMesh::SetName(const std::wstring& name)
{
	m_name = name;
}

std::wstring PolygonMesh::GetName(void)
{
	return m_name;
}
void PolygonMesh::Rotate(float pitch, float yaw, float roll)
{
	m_rotation = { pitch, yaw, roll };
	CalculateLocalMatrix();
}
void PolygonMesh::Translate(float x, float y, float z)
{
	m_translation = { x,y,z };
	CalculateLocalMatrix();
}
void PolygonMesh::Scale(float x, float y, float z)
{
	m_scaling = { x, y, z };
	CalculateLocalMatrix();
}
void PolygonMesh::ResetTransformation()
{
	m_scaling = { 1, 1, 1 };
	m_translation = { 0,0,0 };
	m_rotation = { 0, 0, 0 };
	CalculateLocalMatrix();
}
void PolygonMesh::SetColor(float r, float g, float b, float a)
{
	m_color = { r,g,b,a };
}

void PolygonMesh::CalculateLocalMatrix(void)
{
	DirectX::XMMATRIX scalingMatrix = DirectX::XMMatrixScaling(m_scaling.x, m_scaling.y, m_scaling.z);
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
	DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(m_translation.x, m_translation.y, m_translation.z);
	m_localMatrix = scalingMatrix * rotationMatrix * translationMatrix;
}
DirectX::XMMATRIX PolygonMesh::GetWorldMatrix(void)
{
	return m_worldMatrix;
}
void PolygonMesh::SetIsSeen(bool isSeen)
{
	m_isSeen = isSeen;
}

bool PolygonMesh::IsSeen(void) const
{
	return m_isSeen;
}

IRenderableState PolygonMesh::GetState(void) const
{
	IRenderableState state;
	state.id			= GetID();
	state.isSeen		= m_isSeen;
	state.name			= m_name;
	state.rotation		= m_rotation;
	state.scale			= m_scaling;
	state.translation	= m_translation;
	state.color			= m_color;

	return state;
}