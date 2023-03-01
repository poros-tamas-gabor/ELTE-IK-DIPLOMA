#include "Graphics.h"

Graphics::Graphics() {}
Graphics::~Graphics() {}
bool Graphics::Render() 
{ 
	this->d3dmanager.BeginScene(0.5f, 0.2f, 0.3f, 1.0f);

	this->d3dmanager.EndScene();

	return true;
}
bool Graphics::Initalize(HWND hwnd, float screenWidth, float screenHeight, float screenNear, float screenDepth, bool fullscreen, bool vsync, float fieldOfView)
{
	return this->d3dmanager.Initalize(hwnd, screenWidth, screenHeight, screenNear, screenDepth, fullscreen, vsync, fieldOfView);
}
void Graphics::Shutdown() {
	this->d3dmanager.Shutdown();
}
bool Graphics::Frame() 
{ 
	bool result;

	result = this->Render();

	if (!result)
		return false;

	return true; 
}