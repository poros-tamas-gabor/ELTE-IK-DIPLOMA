#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "Shader.h"
#include "D3DManager.h"
#include <DirectXMath.h>
class Graphics
{
private:
	D3DManager		_d3dmanager;
	VertexShader	_vertexShader;
	PixelShader		_pixelShader;
	bool			Render();
public:

	Graphics();
	Graphics(const Graphics&) = delete;
	~Graphics();
	bool Initalize(HWND hwnd, float screenWidth, float screenHeight, float screenNear, float screenDepth, bool fullscreen = false, bool vsync = true, float fieldOfView = (float)( /*D3DX_PI*/ DirectX::XM_PI / 4.0f));
	void Shutdown();
	bool Frame();


};
#endif