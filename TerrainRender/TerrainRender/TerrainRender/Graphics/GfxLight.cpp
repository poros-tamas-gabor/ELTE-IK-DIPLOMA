#include "GfxLight.h"
#include "../Model/ModelLayer.h"


void GfxLight::SetDiffuseColor(const ModelVector4D& diffuseColor)
{
	this->_diffuseColor.x = diffuseColor.x;
	this->_diffuseColor.y = diffuseColor.y;
	this->_diffuseColor.z = diffuseColor.z;
	this->_diffuseColor.w = diffuseColor.w;
}
void GfxLight::SetInverseDirection(const ModelVector4D& direction)
{
	this->_inverseDirection.x = direction.x;
	this->_inverseDirection.y = direction.y;
	this->_inverseDirection.z = direction.z;
	this->_inverseDirection.w = direction.w;
}
void GfxLight::SetAmbientColor(const ModelVector4D& ambientColor)
{
	this->_ambientColor.x = ambientColor.x;
	this->_ambientColor.y = ambientColor.y;
	this->_ambientColor.z = ambientColor.z;
	this->_ambientColor.w = ambientColor.w;
}

void GfxLight::SetAmbientColor(const DirectX::XMFLOAT4& ambientColor)
{
	this->_ambientColor = ambientColor;
}
void GfxLight::SetDiffuseColor(const DirectX::XMFLOAT4& diffuseColor)
{
	this->_diffuseColor = diffuseColor;
}
void GfxLight::SetInverseDirection(const DirectX::XMFLOAT4& direction)
{
	this->_inverseDirection = direction;
}


void GfxLight::UpdateLightDirection(const ModelLayer* modelLayer)
{
	const ModelLight* modelLight = nullptr;
	modelLayer->GetpModelLight(&modelLight);
	if (modelLight == nullptr)
	{
		return;
	}
	double azimuth = modelLight->GetAzimuth();
	double elevation = modelLight->GetElevation();

	this->SetInverseLightDirection(azimuth, elevation);
}

void GfxLight::SetInverseLightDirection(double azimuth, double elevation)
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

DirectX::XMFLOAT4 GfxLight::GetAmbientColor(void) const
{
	return this->_ambientColor;
}
DirectX::XMFLOAT4 GfxLight::GetDiffuseColor(void) const
{
	return this->_diffuseColor;
}
DirectX::XMFLOAT4 GfxLight::GetInverseDirection(void) const
{
	return this->_inverseDirection;
}