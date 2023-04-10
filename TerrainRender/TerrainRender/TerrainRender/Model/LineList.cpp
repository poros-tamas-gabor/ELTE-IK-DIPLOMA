#include "LineList.h"

#include	"../ErrorHandler.h"
#include	<memory>


bool LineList::Initialize(ID3D11Device* device, IVertexShader* vertexShader, IPixelShader* pixelShader, VertexPolyLine* vertices, unsigned long* indices, UINT vertexCount, UINT indexCount)
{
	if (device == nullptr || vertexShader == nullptr || pixelShader == nullptr)
		return false;

	this->m_pixelShader = pixelShader;
	this->m_vertexShader = vertexShader;
	this->ResetTransformation();

	bool bresult;
	bresult = this->InitializeBuffers(device, vertices, vertexCount);
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
	if (IsSeen())
	{
		m_worldMatrix = m_localMatrix * worldMat;
		bool bresult = this->m_vertexShader->Render(deviceContext, m_worldMatrix, viewMat, projectionMat, m_color);
		if (!bresult)
			THROW_TREXCEPTION(L"Failed to render vertex shader");
		this->RenderBuffers(deviceContext);
		bresult = this->m_pixelShader->Render(deviceContext, this->GetVertexCount(), light);
		if (!bresult)
			THROW_TREXCEPTION(L"Failed to render pixel shader");
	}
}

int LineList::GetIndexCount() const
{
	return 0;
}
int LineList::GetVertexCount() const
{
	return this->_vertexCount;

}

bool LineList::InitializeBuffers(ID3D11Device* device, VertexPolyLine* vertices, UINT vertexCount)
{

	D3D11_BUFFER_DESC						vertexBufferDesc;
	HRESULT									result;
	D3D11_SUBRESOURCE_DATA					vertexData;

	// Set the number of vertices in the vertex array.
	// Set the number of indices in the index array.
	this->_vertexCount = vertexCount;

	try
	{
		// Set up the description of the static vertex buffer.
		ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexPolyLine) * this->_vertexCount;
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
	stride = sizeof(VertexPolyLine);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &this->m_vertexBuffer, &stride, &offset);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

void LineList::SetName(const std::wstring& name)
{
	m_name = name;
}

std::wstring LineList::GetName()
{
	return m_name;
}
void LineList::Rotate(float pitch, float yaw, float roll)
{
	m_rotation = { pitch, yaw, roll };
	CalculateLocalMatrix();
}
void LineList::Translate(float x, float y, float z)
{
	m_translation = { x,y,z };
	CalculateLocalMatrix();
}
void LineList::Scale(float x, float y, float z)
{
	m_scaling = { x, y, z };
	CalculateLocalMatrix();
}
void LineList::ResetTransformation()
{
	m_scaling = { 1, 1, 1 };
	m_translation = { 0,0,0 };
	m_rotation = { 0, 0, 0 };
	CalculateLocalMatrix();
}
void LineList::CalculateLocalMatrix(void)
{
	DirectX::XMMATRIX scalingMatrix = DirectX::XMMatrixScaling(m_scaling.x, m_scaling.y, m_scaling.z);
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
	DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(m_translation.x, m_translation.y, m_translation.z);
	m_localMatrix = scalingMatrix * rotationMatrix * translationMatrix;
}
DirectX::XMMATRIX LineList::GetWorldMatrix(void)
{
	return m_worldMatrix;
}

void LineList::SetColor(float r, float g, float b, float a)
{
	m_color = { r,g,b,a };
}

void LineList::SetIsSeen(bool m_isSeen)
{
	this->m_isSeen = m_isSeen;
}
bool LineList::IsSeen(void) const
{
	return m_isSeen;
}

IRenderableState LineList::GetState(void) const
{
	IRenderableState state;
	state.id = this->GetID();
	state.m_isSeen = m_isSeen;
	state.name = this->m_name;
	state.rotation = this->m_rotation;
	state.scale = this->m_scaling;
	state.translation = this->m_translation;
	state.color = this->m_color;
	return state;
}