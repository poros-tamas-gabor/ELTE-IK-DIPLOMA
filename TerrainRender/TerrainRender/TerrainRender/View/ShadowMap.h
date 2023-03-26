#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include <d3d11.h>
#include <DirectXMath.h>
class ShadowMap
{
private:
	ID3D11DepthStencilView*		m_depthStencilView;
	ID3D11ShaderResourceView*	m_shaderResourceView;
	ID3D11Texture2D*			m_renderTargetTexture;
	ID3D11Texture2D*			m_depthStencilBuffer;
	ID3D11RenderTargetView*		m_renderTargetView;

	int							m_width;
	int							m_height;
	D3D11_VIEWPORT				m_viewport;
	DirectX::XMMATRIX			m_projectionMatrix;
	DirectX::XMMATRIX			m_orthoMatrix;

public:

	ShadowMap();
	~ShadowMap();
	ShadowMap(const ShadowMap& other) = delete;
	ShadowMap& operator= (const ShadowMap& other) = delete;


	bool Initialize(ID3D11Device* device, int textureWidth, int textureHeight, float nearZ, float farZ);
	void Shutdown();

	void SetRenderTarget(ID3D11DeviceContext*);
	void BeginScene(ID3D11DeviceContext*, float, float, float, float);
	ID3D11ShaderResourceView* GetShaderResourceView();
	DirectX::XMMATRIX GetProjectionMatrix(void);
	DirectX::XMMATRIX GetOrthoMatrix(void);

};
#endif // !SHADOW_MAP_H

