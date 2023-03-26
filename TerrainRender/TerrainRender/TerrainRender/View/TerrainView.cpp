#include "TerrainView.h"
#include "../ErrorHandler.h"
TerrainView::TerrainView()  {}
TerrainView::~TerrainView() {}

int SHADOWMAP_WIDTH = 1024;
int SHADOWMAP_HEIGHT = 1024;
float SHADOWMAP_DEPTH = 50.0f;
float SHADOWMAP_NEAR = 1.0f;

bool TerrainView::Initalize(HWND hwnd, float screenWidth, float screenHeight, bool fullscreen, bool vsync)
{
	bool bresult;

	bresult = this->m_d3dView.Initalize(hwnd, screenWidth, screenHeight,  fullscreen, vsync);
	THROW_TREXCEPTION_IF_FAILED(bresult, L"Failed to initialize D3D view");

	bresult = this->m_guiView.Initalize(this->m_d3dView.GetDevice(), this->m_d3dView.GetDeviceContext(),this->m_terrainController);
	THROW_TREXCEPTION_IF_FAILED(bresult, L"Failed to initialize gui view");

	bresult = this->m_shadowMap.Initialize(this->m_d3dView.GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SHADOWMAP_NEAR, SHADOWMAP_DEPTH );
	THROW_TREXCEPTION_IF_FAILED(bresult, L"Failed to initialize shadowmap");

	return true;
}
void TerrainView::Shutdown() 
{
	this->m_d3dView.Shutdown();
	this->m_guiView.Shutdown();
	this->m_shadowMap.Shutdown();
}
bool TerrainView::RenderFrame() 
{ 
	bool result;

	result = this->Render();

	if (!result)
		return false;

	return true; 
}

bool TerrainView::RenderShadowMap()
{
	m_shadowMap.SetRenderTarget(m_d3dView.GetDeviceContext());

	m_shadowMap.BeginScene(m_d3dView.GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	bool result = this->m_terrainModel->RenderShadowMap(m_d3dView.GetDeviceContext());

	this->m_d3dView.SetBackBufferRenderTarget();
	this->m_d3dView.ResetViewport();

	return result;
}

bool TerrainView::Render()
{
	bool bresult;

	//bresult = RenderShadowMap();
	//THROW_TREXCEPTION_IF_FAILED(bresult, L"Failed to render shadow map");

	this->m_d3dView.BeginScene(0.5f, 0.2f, 0.3f, 1.0f);
	this->m_guiView.BeginFrame();

	this->m_terrainModel->Render(m_d3dView.GetDeviceContext(), m_shadowMap.GetShaderResourceView());

	this->m_guiView.ShowSettingWindow();
	
	this->m_guiView.EndFrame();
	this->m_d3dView.EndScene();

	return true;
}


//void TerrainView::ExecuteControl(eventType eventType, const void* data)
//{
//	this->m_terrainController->Control(eventType, data);
//}


ID3D11Device* TerrainView::GetDevice()
{
	return this->m_d3dView.GetDevice();
}
ID3D11DeviceContext* TerrainView::GetDeviceContext()
{
	return this->m_d3dView.GetDeviceContext();
}


void TerrainView::SetController(IController* terrainController)
{
	this->m_terrainController = terrainController;
}
void TerrainView::SetModel(TerrainModel* terrainModel)
{
	this->m_terrainModel = terrainModel;
}

