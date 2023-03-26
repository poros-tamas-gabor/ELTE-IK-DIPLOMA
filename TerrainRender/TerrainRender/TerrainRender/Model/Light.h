#ifndef LIGHT_H
#define LIGHT_H

#include <DirectXMath.h>

class Light
{

private:
	DirectX::XMFLOAT4	_ambientColor = {0.2f, 0.2f, 0.2f, 1.0f};
	DirectX::XMFLOAT4	_diffuseColor = {0.4f, 0.4f, 0.4f, 1.0f};
	DirectX::XMFLOAT4	_inverseDirection = {1.0f, 1.0f, 1.0f, 1.0f};
	DirectX::XMMATRIX	m_orthoProjectionMatrix;
	DirectX::XMMATRIX	m_viewMatrix;

public:
	Light() = default;
	~Light() = default;
	Light(const Light&) = delete;
	Light& operator=(const Light&) = delete;

	void Render();
	//void SetInverseDirection(const DirectX::XMFLOAT4& direction);

	void Initialize(float width, float depthPlane, float nearPlane);
	void SetDiffuseColor(const DirectX::XMFLOAT4& diffuseColor);
	void SetAmbientColor(const DirectX::XMFLOAT4& ambientColor);
	void SetInverseDirectionBySunPosition(double azimuth, double elevation);

	DirectX::XMFLOAT4 GetDiffuseColor( void ) const;
	DirectX::XMFLOAT4 GetAmbientColor( void ) const;
	DirectX::XMFLOAT4 GetInverseDirection( void ) const;

	DirectX::XMMATRIX GetLightViewMatrix(void) const;
	DirectX::XMMATRIX GetLightOrthoProjectionMatrix(void) const;

};

#endif