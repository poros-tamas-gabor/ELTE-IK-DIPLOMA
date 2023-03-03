#include "Graphics.h"

Graphics::Graphics() {}
Graphics::~Graphics() {}
bool Graphics::Render() 
{ 
	this->_d3dmanager.BeginScene(0.5f, 0.2f, 0.3f, 1.0f);





	this->_gfxModel.Render(this->_d3dmanager.GetDeviceContext());

	this->_d3dmanager.GetDeviceContext()->Draw(3, 0);


	//
	//this->_pixelShader.Render(this->_d3dmanager.GetDeviceContext());
	//this->_vertexShader.Render(this->_d3dmanager.GetDeviceContext());


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

	bresult = this->_gfxModel.Initialize(this->_d3dmanager.GetDevice());
	if (!bresult)
	{
		return false;
	}

	return true;
}
void Graphics::Shutdown() 
{
	this->_d3dmanager.Shutdown();
	this->_pixelShader.Shutdown();
	this->_vertexShader.Shutdown();
	this->_gfxModel.Shutdown();
}
bool Graphics::Frame() 
{ 
	bool result;

	result = this->Render();

	if (!result)
		return false;

	return true; 
}