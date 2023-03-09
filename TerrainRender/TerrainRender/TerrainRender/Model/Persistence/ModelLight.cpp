#include "ModelLight.h"


void ModelLight::SetDiffuseColor(float r, float g, float b, float a)
{
	this->_diffuseColor.x = r;
	this->_diffuseColor.y = g;
	this->_diffuseColor.z = b;
	this->_diffuseColor.w = a;
}

void ModelLight::SetAmbientColor(float r, float g, float b, float a)
{
	this->_ambientColor.x = r;
	this->_ambientColor.y = g;
	this->_ambientColor.z = b;
	this->_ambientColor.w = a;
}
void ModelLight::SetDirection(float x, float y, float z, float w)
{
	this->_direction.x = x;
	this->_direction.y = y;
	this->_direction.z = z;
	this->_direction.w = w;
}
ModelVector4D ModelLight::GetDiffuseColor(void) const
{
	return this->_diffuseColor;
}
ModelVector4D ModelLight::GetDirection(void) const
{
	return this->_direction;
}
ModelVector4D ModelLight::GetAmbientColor(void) const
{
	return this->_ambientColor;
}
