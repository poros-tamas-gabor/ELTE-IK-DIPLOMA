#ifndef GFX_LIGHT_H
#define GFX_LIGHT_H

#include <DirectXMath.h>
#include "../Model/ModelLayer.h"

class GfxLight
{

private:
	DirectX::XMFLOAT4 _ambientColor = {0.2f, 0.2f, 0.2f, 1.0f};
	DirectX::XMFLOAT4 _diffuseColor = {0.4f, 0.4f, 0.4f, 1.0f};
	DirectX::XMFLOAT4 _inverseDirection = {1.0f, 1.0f, 1.0f, 1.0f};

public:
	GfxLight() = default;
	~GfxLight() = default;
	GfxLight(const GfxLight&) = delete;
	GfxLight& operator=(const GfxLight&) = delete;


	void SetInverseDirection(const DirectX::XMFLOAT4& direction);
	void SetInverseDirection(const ModelVector4D& diffuseColor);
	
	void SetDiffuseColor(const DirectX::XMFLOAT4& diffuseColor);
	void SetDiffuseColor(const ModelVector4D& diffuseColor);

	void SetAmbientColor(const DirectX::XMFLOAT4& ambientColor);
	void SetAmbientColor(const ModelVector4D& ambientColor);

	void UpdateLightDirection(const ModelLayer* modelLayer);
	void SetInverseLightDirection(double azimuth, double elevation);

	DirectX::XMFLOAT4 GetDiffuseColor( void ) const;
	DirectX::XMFLOAT4 GetAmbientColor( void ) const;
	DirectX::XMFLOAT4 GetInverseDirection( void ) const;
};

#endif