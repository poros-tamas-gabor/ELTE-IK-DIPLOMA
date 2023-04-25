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
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	unsigned int	_vertexCount;
	unsigned int	_indexCount;
	IPixelShaderPtr m_pixelShader;
	IVertexShaderPtr m_vertexShader;

	std::wstring		m_name;
	Vector3D			m_rotation;
	Vector3D			m_scaling;
	Vector3D			m_translation;
	Vector4D			m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMMATRIX	m_localMatrix;
	DirectX::XMMATRIX	m_worldMatrix;
	bool m_isSeen = true;
public:
	PolygonMesh() = default;
	virtual ~PolygonMesh() = default;
	PolygonMesh(const PolygonMesh&) = delete;
	PolygonMesh& operator=(const PolygonMesh&) = delete;

	bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, IVertexShaderPtr vertexShader, IPixelShaderPtr pixelShader, VertexMesh* vertices, unsigned long* indices, UINT vertexCount, UINT indexCount) override;
	void Shutdown() override;
	void Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, DirectX::XMMATRIX worldMat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat,const Light& light) override;
 
	int	GetIndexCount() const;
	int	GetVertexCount() const;

	std::wstring		GetName(void) override;
	DirectX::XMMATRIX	GetWorldMatrix(void) override;
	IRenderableState	GetState(void) const override;

	void SetName(const std::wstring& name) override;
	void SetColor(float r, float g, float b, float a) override;
	void SetIsSeen(bool m_isSeen) override;

	void Rotate(float pitch, float yaw, float roll) override;
	void Translate(float x, float y, float z) override;
	void Scale(float x, float y, float z) override;
	void ResetTransformation() override;
	bool IsSeen(void) const override;

private:
	
	bool InitializeBuffers(Microsoft::WRL::ComPtr<ID3D11Device> device, VertexMesh* vertices, unsigned long* indices, UINT vertexCount, UINT indexCount);
	void ShutdownBuffers();
	void RenderBuffers(Microsoft::WRL::ComPtr<ID3D11DeviceContext>);
	void CalculateLocalMatrix(void);
};



#endif