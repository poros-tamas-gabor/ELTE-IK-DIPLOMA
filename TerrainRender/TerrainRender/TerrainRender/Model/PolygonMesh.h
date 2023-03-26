#ifndef	POLYGON_MESH_H
#define	POLYGON_MESH_H

#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include "../Model/Persistence/ModelStructs.h"
#include "IRenderable.h"
#include "VertexShaderMesh.h"
#include "PixelShaderMesh.h"

class PolygonMesh : public IRenderable<VertexMesh>
{
private:
	ID3D11Buffer*	m_vertexBuffer;
	ID3D11Buffer*	m_indexBuffer;
	unsigned int	_vertexCount;
	unsigned int	_indexCount;
	IPixelShader*	m_pixelShader;
	IVertexShader*	m_vertexShader;

	std::wstring		m_name;
	DirectX::XMFLOAT3	m_rotation;
	DirectX::XMFLOAT3	m_scaling;
	DirectX::XMFLOAT3	m_translation;
	DirectX::XMMATRIX	m_localMatrix;
public:
	PolygonMesh() = default;
	virtual ~PolygonMesh() = default;
	PolygonMesh(const PolygonMesh&) = delete;
	PolygonMesh& operator=(const PolygonMesh&) = delete;


	bool Initialize(ID3D11Device* device, IVertexShader*, IPixelShader*, VertexMesh* vertices, UINT indexCount) override;
	void Shutdown() override;
	void Render(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat,const Light& light) override;
 
	int GetIndexCount() const;
	int GetVertexCount() const;

	void SetName(const std::wstring& name) override;
	std::wstring GetName(void) override;
	void Rotate(float yaw, float pitch, float roll) override;
	void Translate(float x, float y, float z) override;
	void Scale(float x, float y, float z) override;
	void ResetTransformation() override;
	DirectX::XMMATRIX GetLocalMatrix(void) override;

private:
	bool InitializeBuffers(ID3D11Device*, VertexMesh* vertices, UINT indexCount);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	void CalculateLocalMatrix(void);
};



#endif