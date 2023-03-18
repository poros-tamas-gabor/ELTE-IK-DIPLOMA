#ifndef	POLYGON_H
#define	POLYGON_H

#include "IRenderable.h"
#include "VertexPolygon.h"
class PolygonLine : public IRenderable<VertexPolygon>
{
private:
	ID3D11Buffer* m_vertexBuffer;
	unsigned int	_vertexCount;
	IPixelShader* m_pixelShader;
	IVertexShader* m_vertexShader;

public:
	PolygonLine() = default;
	virtual ~PolygonLine() = default;
	PolygonLine(const PolygonLine&) = delete;
	PolygonLine& operator=(const PolygonLine&) = delete;


	bool Initialize(ID3D11Device* device, IVertexShader*, IPixelShader*, VertexPolygon* vertices, UINT indexCount) override;
	void Shutdown() override;
	void Render(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat, const Light& light) override;

	int GetIndexCount() const;
	int GetVertexCount() const;

private:
	bool InitializeBuffers(ID3D11Device*, VertexPolygon* vertices, UINT indexCount);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

};
#endif