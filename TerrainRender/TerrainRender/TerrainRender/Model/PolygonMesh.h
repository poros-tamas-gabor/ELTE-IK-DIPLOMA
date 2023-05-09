#ifndef	POLYGON_MESH_H
#define	POLYGON_MESH_H

///////////////////////////////////////////////////////////////////////////////
// PolygonMesh.h
// =============
//
// The PolygonMesh class is a derived class from the IRenderable interface, specifically using the template parameter of VertexMesh.
// This class is designed to represent a list of triangles that are independent from each other, which together represent a surface.
// 
// By inheriting from the IRenderable interface, the PolygonMesh class provides the necessary methods to render the list of triangles to the screen.
// 
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

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
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	unsigned int						m_vertexCount = 0;
	unsigned int						m_indexCount = 0;
	IPixelShaderPtr						m_pixelShader;
	IVertexShaderPtr					m_vertexShader;

	std::wstring				m_name;
	Vector3D					m_rotation = { 0.0f , 0.0f, 0.0f };
	Vector3D					m_scaling = { 1.0f, 1.0f, 1.0f };
	Vector3D					m_translation = { 0.0f , 0.0f, 0.0f };
	DirectX::XMMATRIX			m_localMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX			m_worldMatrix = DirectX::XMMatrixIdentity();
	Vector4D					m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	bool						m_isSeen = true;

public:
	PolygonMesh() = default;
	virtual ~PolygonMesh() = default;
	PolygonMesh(const PolygonMesh&) = delete;
	PolygonMesh& operator=(const PolygonMesh&) = delete;

	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, IVertexShaderPtr vertexShader, IPixelShaderPtr pixelShader, VertexMesh* vertices, unsigned long* indices, UINT vertexCount, UINT indexCount) override;
	void Shutdown() override;
	void Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, DirectX::XMMATRIX worldMat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat,const Light& light) override;
 
	bool IsSeen(void) const override;
	int	 GetIndexCount() const;
	int  GetVertexCount() const;
	std::wstring		GetName(void) override;
	DirectX::XMMATRIX	GetWorldMatrix(void) override;
	IRenderableState	GetState(void) const override;

	void Rotate(float pitch, float yaw, float roll) override;
	void Translate(float x, float y, float z) override;
	void Scale(float x, float y, float z) override;
	void ResetTransformation() override;

	void SetName(const std::wstring& name) override;
	void SetColor(float r, float g, float b, float a) override;
	void SetIsSeen(bool m_isSeen) override;

private:	
	bool InitializeBuffers(Microsoft::WRL::ComPtr<ID3D11Device> device, VertexMesh* vertices, unsigned long* indices, UINT vertexCount, UINT indexCount);
	void ShutdownBuffers();
	void RenderBuffers(Microsoft::WRL::ComPtr<ID3D11DeviceContext>);
	void CalculateLocalMatrix(void);
};



#endif