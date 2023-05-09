#ifndef LIGHT_H
#define LIGHT_H

#include <DirectXMath.h>
#include <ctime>

///////////////////////////////////////////////////////////////////////////////
// Light.h
// =======
// 
// The Light class represents a parallel lighting in the program. 
// The m_ambientColor and m_diffuseColor store the ambient and diffuse components' color of the light source. 
//
// The UpdateSunPosition method calculates the azimuth and zenith angle values of the Sun for a given geographic coordinate and Unix time.
// CalculateInverseDirectionBySunPosition method calculates the light's direction vector in the orthogonal coordinate system from the determined spherical coordinates.
//
//
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

class Light
{
private:
	DirectX::XMFLOAT4 m_ambientColor = {0.4f, 0.4f, 0.4f, 1.0f};
	DirectX::XMFLOAT4 m_diffuseColor = {0.6f, 0.6f, 0.6f, 1.0f};
	DirectX::XMFLOAT4 m_inverseDirection = {1.0f, 1.0f, 1.0f, 1.0f};

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
	void CalculateInverseDirectionBySunPosition(float azimuth, float elevation);
};

#endif