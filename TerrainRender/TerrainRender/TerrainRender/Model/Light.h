#ifndef LIGHT_H
#define LIGHT_H

#include <DirectXMath.h>
#include "SunPosition.h"

class Light
{

private:
	DirectX::XMFLOAT4 _ambientColor = {0.2f, 0.2f, 0.2f, 1.0f};
	DirectX::XMFLOAT4 _diffuseColor = {0.4f, 0.4f, 0.4f, 1.0f};
	DirectX::XMFLOAT4 _inverseDirection = {1.0f, 1.0f, 1.0f, 1.0f};

	double m_azimuth;
	double m_elevation;

public:
	Light() = default;
	~Light() = default;
	Light(const Light&) = delete;
	Light& operator=(const Light&) = delete;

	void UpdateSunPosition(std::time_t currentEpochTime, double lat, double longitude);
	void SetInverseDirection(const DirectX::XMFLOAT4& direction);
	void SetDiffuseColor(const DirectX::XMFLOAT4& diffuseColor);
	void SetAmbientColor(const DirectX::XMFLOAT4& ambientColor);

	DirectX::XMFLOAT4 GetDiffuseColor( void ) const;
	DirectX::XMFLOAT4 GetAmbientColor( void ) const;
	DirectX::XMFLOAT4 GetInverseDirection( void ) const;
	double GetAzimuth(void) const;
	double GetElevation(void) const;
private:
	void SetInverseDirectionBySunPosition(double azimuth, double elevation);

};

#endif