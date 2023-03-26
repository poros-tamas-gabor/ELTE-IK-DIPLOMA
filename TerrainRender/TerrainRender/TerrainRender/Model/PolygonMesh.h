#ifndef	POLYGON_MESH_H
#define	POLYGON_MESH_H

#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include "../Model/Persistence/ModelStructs.h"
#include "IRenderable.h"
#include "VertexShaderMesh.h"
#include "PixelShaderMesh.h"
#include "Vertex.h"

class PolygonMesh : public IRenderable<VertexMesh>
{
private:
	ID3D11Buffer*	m_vertexBuffer;
	ID3D11Buffer*	m_indexBuffer;
	unsigned int	_vertexCount;
	unsigned int	_indexCount;

public:
	PolygonMesh() = default;
	virtual ~PolygonMesh() = default;
	PolygonMesh(const PolygonMesh&) = delete;
	PolygonMesh& operator=(const PolygonMesh&) = delete;


	bool Initialize(ID3D11Device* device,  VertexMesh* vertices, UINT indexCount) override;
	void Shutdown() override;
	void Render(ID3D11DeviceContext* deviceContext, IVertexShader& vertexShader, IPixelShader& pixelshader, DirectX::XMMATRIX worldMat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat,const Light& light) override;
 
	int GetIndexCount() const;
	int GetVertexCount() const;

private:
	bool InitializeBuffers(ID3D11Device*, VertexMesh* vertices, UINT indexCount);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
};



#endif