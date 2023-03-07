#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "VertexShader.h"
#include "PixelShader.h"
#include "D3DManager.h"
#include "GfxModel.h"
#include "GfxCamera.h"
#include <DirectXMath.h>
#include "Position.h"
#include "../Model/ModelLayer.h"
#include <vector>
#include <memory>
class Graphics
{
private:
	D3DManager							_d3dmanager;
	VertexShader						_vertexShader;
	PixelShader							_pixelShader;
	GfxCamera							_camera;
	GfxModel							_gfxModel;

	std::shared_ptr<ModelLayer>			_model;

	bool			Render();
public:

	Graphics(std::shared_ptr<ModelLayer>&);
	Graphics(const Graphics&) = delete;
	~Graphics();
	bool Initalize(HWND hwnd, float screenWidth, float screenHeight, float screenNear, float screenDepth, bool fullscreen = false, bool vsync = true, float fieldOfView = (float)( /*D3DX_PI*/ DirectX::XM_PI / 4.0f));
	void Shutdown();
	bool Frame();
	void Update();
	Position				_position;


};
#endif