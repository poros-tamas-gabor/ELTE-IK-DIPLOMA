#include "Light.h"


void Light::Initialize(float width, float nearZ, float farZ)
{
	m_orthoProjectionMatrix = DirectX::XMMatrixOrthographicLH(width, width, nearZ, farZ);
}
void Light::SetAmbientColor(const DirectX::XMFLOAT4& ambientColor)
{
	this->_ambientColor = ambientColor;
}
void Light::SetDiffuseColor(const DirectX::XMFLOAT4& diffuseColor)
{
	this->_diffuseColor = diffuseColor;
}
//void Light::SetInverseDirection(const DirectX::XMFLOAT4& direction)
//{
//	this->_inverseDirection = direction;
//}

void Light::Render()
{
	// Setup the position of the camera in the world.
	DirectX::XMVECTOR lightPosition = DirectX::XMVectorSet(- _inverseDirection.x,- _inverseDirection.y, - _inverseDirection.z, _inverseDirection.w);
	m_viewMatrix = DirectX::XMMatrixLookAtLH(lightPosition, DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
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

DirectX::XMMATRIX Light::GetLightViewMatrix(void) const
{
	return m_viewMatrix;
}
DirectX::XMMATRIX Light::GetLightOrthoProjectionMatrix(void) const
{
	return m_orthoProjectionMatrix;
}