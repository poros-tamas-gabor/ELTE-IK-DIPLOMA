#ifndef LIGHT_H
#define LIGHT_H

#include <DirectXMath.h>
#include <ctime>

class Light
{

private:
	DirectX::XMFLOAT4 _ambientColor = {0.4f, 0.4f, 0.4f, 1.0f};
	DirectX::XMFLOAT4 _diffuseColor = {0.6f, 0.6f, 0.6f, 1.0f};
	DirectX::XMFLOAT4 _inverseDirection = {1.0f, 1.0f, 1.0f, 1.0f};

	float m_azimuth;
	float m_elevation;

public:
	Light() = default;
	~Light() = default;
	Light(const Light&) = delete;
	Light& operator=(const Light&) = delete;

	void UpdateSunPosition(std::time_t currentEpochTime, float lat, float longitude);

	void SetDiffuseColor(const DirectX::XMFLOAT4& diffuseColor);
	void SetAmbientColor(const DirectX::XMFLOAT4& ambientColor);

	DirectX::XMFLOAT4	GetDiffuseColor( void ) const;
	DirectX::XMFLOAT4	GetAmbientColor( void ) const;
	DirectX::XMFLOAT4	GetInverseDirection( void ) const;
	float				GetAzimuth(void) const;
	float				GetElevation(void) const;
private:
	void SetInverseDirectionBySunPosition(float azimuth, float elevation);

};

#endif