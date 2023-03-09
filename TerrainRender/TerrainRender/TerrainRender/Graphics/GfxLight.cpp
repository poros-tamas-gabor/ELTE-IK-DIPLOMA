#include "GfxLight.h"
#include "../Model/ModelLayer.h"


void GfxLight::SetDiffuseColor(const ModelVector4D& diffuseColor)
{
	this->_diffuseColor.x = diffuseColor.x;
	this->_diffuseColor.y = diffuseColor.y;
	this->_diffuseColor.z = diffuseColor.z;
	this->_diffuseColor.w = diffuseColor.w;
}
void GfxLight::SetDirection(const ModelVector4D& direction)
{
	this->_direction.x = direction.x;
	this->_direction.y = direction.y;
	this->_direction.z = direction.z;
	this->_direction.w = direction.w;
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
void GfxLight::SetDirection(const DirectX::XMFLOAT4& direction)
{
	this->_direction = direction;
}


void GfxLight::SetLightParameters(const ModelLayer* modelLayer)
{
	const ModelLight* modelLight = nullptr;
	modelLayer->GetLight(&modelLight);
	if (modelLight == nullptr)
	{
		return;
	}
	ModelVector4D diffuseColor = modelLight->GetDiffuseColor();
	ModelVector4D ambientColor = modelLight->GetAmbientColor();
	ModelVector4D direction = modelLight->GetDirection();

	this->SetAmbientColor(ambientColor);
	this->SetDiffuseColor(diffuseColor);
	this->SetDirection(direction);
}


DirectX::XMFLOAT4 GfxLight::GetAmbientColor(void) const
{
	return this->_ambientColor;
}
DirectX::XMFLOAT4 GfxLight::GetDiffuseColor(void) const
{
	return this->_diffuseColor;
}
DirectX::XMFLOAT4 GfxLight::GetDirection(void) const
{
	return this->_direction;
}