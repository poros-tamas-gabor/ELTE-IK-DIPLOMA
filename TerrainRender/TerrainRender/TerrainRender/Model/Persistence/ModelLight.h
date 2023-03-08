#ifndef MODEL_LIGHT_H
#define MODEL_LIGHT_H

#include "ModelVertex.h"

class ModelLight
{
private:
	ModelVector4D _diffuseColor;
	ModelVector4D _direction;

public:

	ModelLight() = default;

	void SetDiffuseColor(float r, float g, float b, float a);
	void SetDirection	(float r, float g, float b, float a);
	ModelVector4D GetDiffuseColor(void) const;
	ModelVector4D GetDirection(void) const;

};

#endif