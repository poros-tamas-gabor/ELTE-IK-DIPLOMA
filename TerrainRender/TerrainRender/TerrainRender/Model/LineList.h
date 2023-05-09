#ifndef	LINE_LIST_H
#define	LINE_LIST_H

///////////////////////////////////////////////////////////////////////////////
// LineList.h
// ==========
//
// The LineList class is a derived class from the IRenderable interface, specifically using the template parameter of VertexPolyLine.
// This class is designed to represent a list of lines that are independent from each other.
// 
// By inheriting from the IRenderable interface, the LineList class provides the necessary methods to render the list of lines to the screen.
// 
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

#include "IRenderable.h"
class LineList : public IRenderable<VertexPolyLine>
{
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer>m_vertexBuffer;
	unsigned int						m_vertexCount;
	IPixelShaderPtr						m_pixelShader;
	IVertexShaderPtr					m_vertexShader;

	std::wstring						m_name;
	Vector3D							m_rotation		= { 0.0f , 0.0f, 0.0f };
	Vector3D							m_scaling		= { 1.0f, 1.0f, 1.0f };
	Vector3D							m_translation	= { 0.0f , 0.0f, 0.0f };
	DirectX::XMMATRIX					m_localMatrix	= DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX					m_worldMatrix	= DirectX::XMMatrixIdentity();
	Vector4D							m_color			= {1.0f, 1.0f, 1.0f, 1.0f};
	bool								m_isSeen		= true;

public:
	LineList() = default;
	virtual ~LineList() = default;
	LineList(const LineList&) = delete;
	LineList& operator=(const LineList&) = delete;

	virtual bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, IVertexShaderPtr vertexShader, IPixelShaderPtr pixelShader, VertexPolyLine* vertices, unsigned long* indices, UINT vertexCount, UINT indexCount) override;
	void Shutdown() override;
	void Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, DirectX::XMMATRIX worldMat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat, const Light& light) override;
 
	bool IsSeen(void) const override;
	int	 GetIndexCount() const;
	int  GetVertexCount() const;
	std::wstring		GetName(void) override;
	DirectX::XMMATRIX	GetWorldMatrix(void) override;
	IRenderableState	GetState(void) const override;

	void Rotate(float pitch, float yaw, float roll) override;
	void Translate(float x, float y, float z) override;
	void Scale(float x, float y, float z) override;
	void ResetTransformation(void) override;

	void SetName(const std::wstring& name) override;
	void SetColor(float r, float g, float b, float a) override;
	void SetIsSeen(bool m_isSeen) override;

protected:
	bool InitializeBuffers(Microsoft::WRL::ComPtr<ID3D11Device>, VertexPolyLine* vertices,UINT indexCount);
	void ShutdownBuffers();
	virtual void RenderBuffers(Microsoft::WRL::ComPtr<ID3D11DeviceContext>);
	void CalculateLocalMatrix(void);

};
#endif