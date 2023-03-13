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
#include "GfxLight.h"
#include "IObserver.h"
#include "ImGuiManager.h"
#include <vector>
#include <memory>

class IController;
class GuiController;
class Graphics : public IObserver
{
private:
	D3DManager			_d3dmanager;
	VertexShader		_vertexShader;
	PixelShader			_pixelShader;
	GfxCamera			_camera;
	GfxModel			_gfxModel;
	GfxLight			_gfxLight;
	ImGuiManager		_imgui;

	ModelLayer*			_model;
	const IController*		_controller;
	const GuiController*		_guiController;
	



	bool			Render();
public:

	Graphics();
	Graphics(const Graphics&) = delete;
	~Graphics();
	bool Initalize(HWND hwnd, float screenWidth, float screenHeight, float screenNear, float screenDepth, bool fullscreen = false, bool vsync = true, float fieldOfView = (float)( /*D3DX_PI*/ DirectX::XM_PI / 4.0f));
	void Shutdown();
	bool Frame();
	void Update() override;
	void doControl(float dt);
	void SetModel(ModelLayer* model);
	void SetGuiController(const GuiController* guiController);
	void SetModeController(const IController* controller);
	Position				_position;


};
#endif