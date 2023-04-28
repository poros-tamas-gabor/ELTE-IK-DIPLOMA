#include "Light.h"
#include "../sunPos/SunPos.h"
#include <ctime>

void Light::SetAmbientColor(const DirectX::XMFLOAT4& ambientColor)
{
	this->_ambientColor = ambientColor;
}
void Light::SetDiffuseColor(const DirectX::XMFLOAT4& diffuseColor)
{
	this->_diffuseColor = diffuseColor;
}

void Light::UpdateSunPosition(std::time_t currentEpochTime, double lat, double longitude)
{
	struct tm buf;
	gmtime_s(&buf, &currentEpochTime);
	cTime time = { buf.tm_year + 1900, buf.tm_mon + 1, buf.tm_mday, buf.tm_hour, buf.tm_min };
	cSunCoordinates sunCoords;
	sunpos(time, { longitude, lat }, &sunCoords);

	m_azimuth = sunCoords.dAzimuth /180.0 * pi;
	m_elevation = pi / 2 - (sunCoords.dZenithAngle / 180.0 * pi);

	SetInverseDirectionBySunPosition(m_azimuth, m_elevation);
}

void Light::SetInverseDirectionBySunPosition(double azimuth, double elevation)
{
	float rotXRad = -elevation;
	float rotYRad = azimuth;
	float rotZRad = 0;
	// Create the rotation matrix from the yaw, pitch, and roll values.
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotXRad, rotYRad, rotZRad);
	const DirectX::XMVECTOR defaultZVector = DirectX::XMVectorSet(0.0, 0.0, 1.0, 0.0);
	DirectX::XMVECTOR inverseLightDirection = DirectX::XMVector4Transform(defaultZVector, rotationMatrix);
	DirectX::XMStoreFloat4(&this->_inverseDirection, inverseLightDirection);
}

DirectX::XMFLOAT4 Light::GetAmbientColor(void) const
{
	return this->_ambientColor;
}
DirectX::XMFLOAT4 Light::GetDiffuseColor(void) const
{
	return this->_diffuseColor;
}
DirectX::XMFLOAT4 Light::GetInverseDirection(void) const
{
	return this->_inverseDirection;
}

double Light::GetAzimuth(void) const
{
	return m_azimuth;
}
double Light::GetElevation(void) const
{
	return m_elevation;
}