#include "Graphics.h"

Graphics::Graphics(std::shared_ptr<ModelLayer>& model) : _model(model) {}
Graphics::~Graphics() {}


void Graphics::Update()
{


}

bool Graphics::Render() 
{ 
	bool bresult;
	this->_d3dmanager.BeginScene(0.5f, 0.2f, 0.3f, 1.0f);



	this->_camera.SetProjectionValues(45, (float)600 / 800, 1, 100);

	
	DirectX::XMMATRIX mat = DirectX::XMMatrixIdentity();



	this->_camera.Render();
	DirectX::XMMATRIX projectionMat;

	DirectX::XMMATRIX viewMat;
	this->_camera.GetViewMatrix(viewMat);
	this->_camera.GetProjectionMatrix(projectionMat);
	
	bresult = this->_vertexShader.Render(this->_d3dmanager.GetDeviceContext(), mat, viewMat , projectionMat);
	if (!bresult)
	{
		return false;
	}


	this->_gfxModel.Render(this->_d3dmanager.GetDeviceContext());
	this->_pixelShader.Render(this->_d3dmanager.GetDeviceContext(), this->_gfxModel.GetVertexCount());
	


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

	if (!bresult)
	{
		return false;
	}
	bresult = _gfxModel.Initialize(this->_d3dmanager.GetDevice(), this->_model.get());
	if (!bresult)
	{
		return false;
	}

	this->_position.SetCamera(&this->_camera);
	

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