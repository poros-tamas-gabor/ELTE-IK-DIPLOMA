#include "Graphics.h"
#include "../Controller/Controllers.h"
Graphics::Graphics()  {}
Graphics::~Graphics() {}


void Graphics::Update()
{
	//TODO::
	bool bresult;
	bresult = _gfxModel.Initialize(this->_d3dmanager.GetDevice(), this->_model);



}

void Graphics::doControl(float dt)
{
	this->_controller->Control(dt, this);
}

bool Graphics::Render() 
{ 
	bool bresult;
	this->_d3dmanager.BeginScene(0.5f, 0.2f, 0.3f, 1.0f);
	this->_imgui.BeginFrame();



	this->_camera.SetProjectionValues(45, (float)600 / 800, 1, 100);

	
	DirectX::XMMATRIX mat = DirectX::XMMatrixIdentity();



	this->_camera.Render();
	DirectX::XMMATRIX projectionMat;

	DirectX::XMMATRIX viewMat;
	this->_camera.GetViewMatrix(viewMat);
	this->_camera.GetProjectionMatrix(projectionMat);

	DirectX::XMFLOAT4 ambientColor = this->_gfxLight.GetAmbientColor();
	DirectX::XMFLOAT4 diffusecolor = this->_gfxLight.GetDiffuseColor();
	DirectX::XMFLOAT4 dir = this->_gfxLight.GetInverseDirection();
	
	bresult = this->_vertexShader.Render(this->_d3dmanager.GetDeviceContext(), mat, viewMat , projectionMat, ambientColor, diffusecolor, dir);
	if (!bresult)
	{
		return false;
	}


	this->_gfxModel.Render(this->_d3dmanager.GetDeviceContext());
	this->_pixelShader.Render(this->_d3dmanager.GetDeviceContext(), this->_gfxModel.GetVertexCount());
	
	this->_imgui.ShowWindow();
	this->_imgui.EndFrame();
	this->_d3dmanager.EndScene();

	return true;
}
bool Graphics::Initalize(HWND hwnd, float screenWidth, float screenHeight, float screenNear, float screenDepth, bool fullscreen, bool vsync, float fieldOfView)
{
	bool bresult;

	bresult = this->_d3dmanager.Initalize(hwnd, screenWidth, screenHeight, screenNear, screenDepth, fullscreen, vsync, fieldOfView);
	if (!bresult)
	{
		return false;
	}

	bresult = this->_vertexShader.Initialize(this->_d3dmanager.GetDevice(), hwnd);
	if (!bresult)
	{
		return false;
	}

	bresult = this->_pixelShader.Initialize(this->_d3dmanager.GetDevice(), hwnd);
	if (!bresult)
	{
		return false;
	}

	_gfxLight.UpdateLightDirection(this->_model);

	this->_position.SetCamera(&this->_camera);
	
	this->_imgui.Initalize(this->_d3dmanager.GetDevice(), this->_d3dmanager.GetDeviceContext(),this->_guiController);

	return true;
}
void Graphics::Shutdown() 
{
	this->_d3dmanager.Shutdown();
	this->_pixelShader.Shutdown();
	this->_vertexShader.Shutdown();
	this->_gfxModel.Shutdown();
	this->_model->Shutdown();
	this->_imgui.Shutdown();
}
bool Graphics::Frame() 
{ 
	bool result;

	result = this->Render();

	if (!result)
		return false;

	return true; 
}

void Graphics::SetModel(ModelLayer* model)
{
	this->_model = model;
}
void Graphics::SetGuiController(const GuiController* guiController)
{
	this->_guiController = guiController;
}
void Graphics::SetModeController(const IController* controller)
{
	this->_controller = controller;
}