#include "CameraTrajectory.h"

bool CameraTrajectory::Initialize(ID3D11Device* device, IVertexShader* vertexShader, IPixelShader* pixelShader, const std::vector<CameraPose>& cameraPoses)
{
	if (device == nullptr || vertexShader == nullptr || pixelShader == nullptr)
		return false;


	std::vector<VertexPolygon> vertices;

	for (CameraPose camerapose : cameraPoses)
	{
		VertexPolygon vertex;

		vertex.position = { (float)camerapose.east,(float)-camerapose.down,(float)camerapose.north };
		vertex.color = { 1.0f, 0.0f, 1.0f, 0.0f };
		vertices.push_back(vertex);
	}

	return this->Initialize(device, vertexShader, pixelShader, &vertices[0], vertices.size());
} 

bool CameraTrajectory::Initialize(ID3D11Device* device, IVertexShader* vertexShader, IPixelShader* pixelShader, VertexPolygon* vertices, UINT indexCount)
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

void CameraTrajectory::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	//set vertex buffer stride and offset
	stride = sizeof(VertexPolygon);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &this->m_vertexBuffer, &stride, &offset);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
}