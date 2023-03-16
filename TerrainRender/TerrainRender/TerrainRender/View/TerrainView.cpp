#include "TerrainView.h"
TerrainView::TerrainView()  {}
TerrainView::~TerrainView() {}


bool TerrainView::Initalize(HWND hwnd, float screenWidth, float screenHeight, bool fullscreen, bool vsync)
{
	bool bresult;

	bresult = this->m_d3dView.Initalize(hwnd, screenWidth, screenHeight,  fullscreen, vsync);
	if (!bresult)
	{
		return false;
	}

	this->m_guiView.Initalize(this->m_d3dView.GetDevice(), this->m_d3dView.GetDeviceContext(),this->m_terrainController);

	return true;
}
void TerrainView::Shutdown() 
{
	this->m_d3dView.Shutdown();
	this->m_guiView.Shutdown();
}
bool TerrainView::RenderFrame() 
{ 
	bool result;

	result = this->Render();

	if (!result)
		return false;

	return true; 
}

bool TerrainView::Render()
{
	bool bresult;
	this->m_d3dView.BeginScene(0.5f, 0.2f, 0.3f, 1.0f);
	this->m_guiView.BeginFrame();

	this->m_terrainModel->Render(m_d3dView.GetDeviceContext());

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

