#include "TerrainView.h"
#include "../ErrorHandler.h"
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

	this->m_guiView.Initalize(this->m_d3dView.GetDevice().Get(), this->m_d3dView.GetDeviceContext().Get(), this->m_terrainController);

	return true;
}
bool TerrainView::Resize(unsigned screenWidth, unsigned screenHeight)
{
	bool result;
	result  = m_d3dView.Resize(screenWidth, screenHeight);
	return result;
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

bool TerrainView::CaptureScreen(unsigned frameNum)
{
	try
	{
		THROW_TREXCEPTION_IF_FAILED(!m_outputDirectoryPath.empty(), L"Failed to capture screen because the output directory was not choose");
		this->m_d3dView.CaptureScreen(m_outputDirectoryPath, frameNum);
	}
	catch (const TRException& e)
	{
		ErrorHandler::Log(e);
	}
	catch (const std::exception& e)
	{
		ErrorHandler::Log(e);
	}

	return false;
}

void TerrainView::SetOutputDirectory(const std::wstring& m_outputDirectoryPath)
{
	this->m_outputDirectoryPath = m_outputDirectoryPath;
	this->m_guiView.SetOutputDirectory(m_outputDirectoryPath);
}
bool TerrainView::Render()
{
	bool bresult;
	this->m_d3dView.BeginScene(m_backgroungColor.x, m_backgroungColor.y, m_backgroungColor.z, 1.0f);
	this->m_guiView.BeginFrame();

	this->m_terrainModel->Render(m_d3dView.GetDeviceContext().Get());

	this->m_guiView.ShowWindows();
	
	this->m_guiView.EndFrame();
	this->m_d3dView.EndScene();

	return true;
}

Microsoft::WRL::ComPtr<ID3D11Device> TerrainView::GetDevice()
{
	return this->m_d3dView.GetDevice();
}
Microsoft::WRL::ComPtr<ID3D11DeviceContext> TerrainView::GetDeviceContext()
{
	return this->m_d3dView.GetDeviceContext();
}


void TerrainView::SetController(IControllerPtr terrainController)
{
	this->m_terrainController = terrainController;
}
void TerrainView::SetModel(IModelPtr terrainModel)
{
	this->m_terrainModel = terrainModel;
}

void TerrainView::HandleIModelState(const std::vector<IRenderableState>& state) {
	this->m_guiView.HandleIModelState(state);
}

void TerrainView::HandleIModelState(const FlythroughState& state)
{
	this->m_guiView.HandleIModelState(state);
}
void TerrainView::HandleIModelState(const Explore3DState& state) 
{
	this->m_guiView.HandleIModelState(state);
}
void TerrainView::HandleIModelState(const CameraState& state)
{
	this->m_guiView.HandleIModelState(state);
}

void TerrainView::Help()
{
	this->m_guiView.Help();
}
