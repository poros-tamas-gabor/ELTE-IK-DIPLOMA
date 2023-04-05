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
	unsigned int	_vertexCount;
	IPixelShader*	m_pixelShader;
	IVertexShader*	m_vertexShader;

	std::wstring		m_name;
	DirectX::XMFLOAT3	m_rotation;
	DirectX::XMFLOAT3	m_scaling;
	DirectX::XMFLOAT3	m_translation;
	DirectX::XMMATRIX	m_localMatrix;
	DirectX::XMMATRIX	m_worldMatrix;
	DirectX::XMFLOAT4	m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	bool m_isSeen = true;
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
	void Rotate(float pitch, float yaw, float roll) override;
	void Translate(float x, float y, float z) override;
	void Scale(float x, float y, float z) override;
	void ResetTransformation() override;
	DirectX::XMMATRIX GetWorldMatrix(void) override;
	void SetColor(float r, float g, float b, float a) override;
	void SetIsSeen(bool m_isSeen) override;
	bool IsSeen(void) const override;
	IRenderableState	GetState(void) const override;

private:
	bool InitializeBuffers(ID3D11Device*, VertexMesh* vertices, UINT indexCount);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	void CalculateLocalMatrix(void);
};



#endif