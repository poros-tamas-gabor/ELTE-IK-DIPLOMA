#include "LineList.h"
#include	"../ErrorHandler.h"
#include	<memory>


bool LineList::Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, IVertexShaderPtr vertexShader, IPixelShaderPtr pixelShader, VertexPolyLine* vertices, unsigned long* indices, UINT vertexCount, UINT indexCount)
{
	bool bresult;

	if (device == nullptr || vertexShader == nullptr || pixelShader == nullptr || vertices == nullptr || vertexCount == 0)
		return false;

	m_pixelShader = pixelShader;
	m_vertexShader = vertexShader;
	ResetTransformation();

	bresult = InitializeBuffers(device, vertices, vertexCount);

	return bresult;
}
void LineList::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();
}
void LineList::Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, DirectX::XMMATRIX worldMat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat, const Light& light)
{
	bool bresult;

	try 
	{
		if (IsSeen())
		{
			m_worldMatrix = m_localMatrix * worldMat;
			bresult = m_vertexShader->Render(deviceContext, m_worldMatrix, viewMat, projectionMat, m_color.ToXMFLOAT4());
			THROW_TREXCEPTION_IF_FAILED(bresult, L"Failed to render vertex shader");

			RenderBuffers(deviceContext);
			bresult = m_pixelShader->Render(deviceContext, GetVertexCount(), light);
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

int LineList::GetIndexCount() const
{
	return 0;
}
int LineList::GetVertexCount() const
{
	return m_vertexCount;
}

bool LineList::InitializeBuffers(Microsoft::WRL::ComPtr<ID3D11Device> device, VertexPolyLine* vertices, UINT vertexCount)
{

	D3D11_BUFFER_DESC						vertexBufferDesc;
	HRESULT									result;
	D3D11_SUBRESOURCE_DATA					vertexData;

	// Set the number of vertices in the vertex array.
	m_vertexCount = vertexCount;

	try
	{
		// Set up the description of the static vertex buffer.
		ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexPolyLine) * m_vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		result = device->CreateBuffer(&vertexBufferDesc, &vertexData, m_vertexBuffer.ReleaseAndGetAddressOf() );
		
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
	return;
}
void LineList::RenderBuffers(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	//set vertex buffer stride and offset
	stride = sizeof(VertexPolyLine);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

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
	//THROW_TREXCEPTION(L"Not implemented function");
	//m_scaling = { x, y, z };
	//CalculateLocalMatrix();
}
void LineList::ResetTransformation()
{
	m_rotation		= { 0.0f , 0.0f, 0.0f };
	m_scaling		= { 1.0f, 1.0f, 1.0f };
	m_translation	= { 0.0f , 0.0f, 0.0f };
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

void LineList::SetIsSeen(bool isSeen)
{
	m_isSeen = isSeen;
}

bool LineList::IsSeen(void) const
{
	return m_isSeen;
}

IRenderableState LineList::GetState(void) const
{
	IRenderableState state;
	state.id = GetID();
	state.isSeen = m_isSeen;
	state.name = m_name;
	state.rotation = m_rotation;
	state.scale = m_scaling;
	state.translation = m_translation;
	state.color = m_color;
	return state;
}