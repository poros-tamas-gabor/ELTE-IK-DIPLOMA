#ifndef GFX_LIGHT_H
#define GFX_LIGHT_H

#include <DirectXMath.h>
#include "../Model/ModelLayer.h"

class GfxLight
{

private:
	DirectX::XMFLOAT4 _diffuseColor;
	DirectX::XMFLOAT4 _direction;

public:
	GfxLight() = default;
	~GfxLight() = default;
	GfxLight(const GfxLight&) = delete;
	GfxLight& operator=(const GfxLight&) = delete;


	void SetDirection(const DirectX::XMFLOAT4& direction);
	void SetDirection(const ModelVector4D& diffuseColor);
	
	void SetDiffuseColor(const DirectX::XMFLOAT4& diffuseColor);
	void SetDiffuseColor(const ModelVector4D& diffuseColor);

	void SetLightParameters(const ModelLayer* modelLayer);

	DirectX::XMFLOAT4 GetDiffuseColor( void ) const;
	DirectX::XMFLOAT4 GetDirection( void ) const;
};

#endif