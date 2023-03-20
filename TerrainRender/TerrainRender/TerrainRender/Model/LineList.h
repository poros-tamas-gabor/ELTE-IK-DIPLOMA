#ifndef	LINE_LIST_H
#define	LINE_LIST_H

#include "IRenderable.h"
#include "VertexPolygon.h"
class LineList : public IRenderable<VertexPolygon>
{
protected:
	ID3D11Buffer* m_vertexBuffer;
	unsigned int	_vertexCount;
	IPixelShader* m_pixelShader;
	IVertexShader* m_vertexShader;

public:
	LineList() = default;
	virtual ~LineList() = default;
	LineList(const LineList&) = delete;
	LineList& operator=(const LineList&) = delete;


	virtual bool Initialize(ID3D11Device* device, IVertexShader*, IPixelShader*, VertexPolygon* vertices, UINT indexCount) override;
	void Shutdown() override;
	void Render(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat, const Light& light) override;
 
	int GetIndexCount() const;
	int GetVertexCount() const;

protected:
	bool InitializeBuffers(ID3D11Device*, VertexPolygon* vertices, UINT indexCount);
	void ShutdownBuffers();
	virtual void RenderBuffers(ID3D11DeviceContext*);

};
#endif