#ifndef	LINE_LIST_H
#define	LINE_LIST_H

#include "IRenderable.h"
class LineList : public IRenderable<VertexPolyLine>
{
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_vertexBuffer;
	unsigned int		_vertexCount;
	IPixelShaderPtr		m_pixelShader;
	IVertexShaderPtr		m_vertexShader;

	std::wstring		m_name;
	DirectX::XMFLOAT3	m_rotation;
	DirectX::XMFLOAT3	m_scaling;
	DirectX::XMFLOAT3	m_translation;
	DirectX::XMMATRIX	m_localMatrix;
	DirectX::XMMATRIX	m_worldMatrix;
	DirectX::XMFLOAT4	m_color = {1.0f, 1.0f, 1.0f, 1.0f};
	bool				m_isSeen = true;

public:
	LineList() = default;
	virtual ~LineList() = default;
	LineList(const LineList&) = delete;
	LineList& operator=(const LineList&) = delete;


	virtual bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device> device, IVertexShaderPtr vertexShader, IPixelShaderPtr pixelShader, VertexPolyLine* vertices, size_t* indices, size_t vertexCount, size_t indexCount) override;
	void Shutdown() override;
	void Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext, DirectX::XMMATRIX worldMat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat, const Light& light) override;
 
	int GetIndexCount() const;
	int GetVertexCount() const;

	void SetName(const std::wstring& name) override;
	std::wstring GetName(void) override;
	void Rotate(float pitch, float yaw, float roll) override;
	void Translate(float x, float y, float z) override;
	void Scale(float x, float y, float z) override;
	void ResetTransformation() override;
	DirectX::XMMATRIX GetWorldMatrix(void) override;
	void SetColor( float r, float g, float b, float a) override;
	void SetIsSeen(bool m_isSeen) override;
	bool IsSeen(void) const override;
	IRenderableState	GetState(void) const override;

protected:
	bool InitializeBuffers(Microsoft::WRL::ComPtr<ID3D11Device>, VertexPolyLine* vertices, size_t indexCount);
	void ShutdownBuffers();
	virtual void RenderBuffers(Microsoft::WRL::ComPtr<ID3D11DeviceContext>);
	void CalculateLocalMatrix(void);

};
#endif