#include "TerrainView.h"
#include "../ErrorHandler.h"
TerrainView::TerrainView()  {}
TerrainView::~TerrainView() {}


bool TerrainView::Initalize(HWND hwnd, float screenWidth, float screenHeight, bool fullscreen, bool vsync)
{
	try 
	{
		bool bresult;

		bresult = m_d3dView.Initalize(hwnd, screenWidth, screenHeight, fullscreen, vsync);
		if (!bresult)
		{
			return false;
		}

		m_guiView.Initalize(m_d3dView.GetDevice().Get(), m_d3dView.GetDeviceContext().Get(), m_terrainController);

		return true;
	}
	catch (const COMException& e)
	{
		ErrorHandler::Log(e);
	}
	catch (const TRException& e)
	{
		ErrorHandler::Log(e);
	}
	catch (const std::exception& e)
	{
		ErrorHandler::Log(e);
	}
	catch (...)
	{
		ErrorHandler::Log("Unknown Exception: No details available");
	}
	return false;
}

bool TerrainView::Resize(unsigned screenWidth, unsigned screenHeight)
{
	try
	{
		bool result;
		result = m_d3dView.Resize(screenWidth, screenHeight);
		return result;
	}
	catch (const COMException& e)
	{
		ErrorHandler::Log(e);
	}
	catch (const TRException& e)
	{
		ErrorHandler::Log(e);
	}
	catch (const std::exception& e)
	{
		ErrorHandler::Log(e);
	}
	catch (...)
	{
		ErrorHandler::Log("Unknown Exception: No details available");
	}
	return false;


}

void TerrainView::Shutdown() 
{
	m_d3dView.Shutdown();
	m_guiView.Shutdown();
}
bool TerrainView::RenderFrame() 
{ 
	try
	{
		bool result;

		result = Render();

		if (!result)
			return false;

		return true;
	}
	catch (const COMException& e)
	{
		ErrorHandler::Log(e);
	}
	catch (const TRException& e)
	{
		ErrorHandler::Log(e);
	}
	catch (const std::exception& e)
	{
		ErrorHandler::Log(e);
	}
	catch (...)
	{
		ErrorHandler::Log("Unknown Exception: No details available");
	}
	return false;

}

bool TerrainView::HandleMessage(IViewMessageIDs message, const std::vector<std::wstring>& stringParams, const std::vector<float>& fparams, const std::vector<unsigned>& uparams)
{
	try
	{
		switch (message)
		{
		case IDV_INVALID:
			return false;
		case IDV_SET_OUTPUT_DIRECTORY:
			SetOutputDirectory(stringParams.at(0));
			return true;
		case IDV_SHOW_HELP:
			ShowHelp();
			return true;
		case IDV_SHOW_GENERAL:
			ShowGeneralWindow();
			return true;
		case IDV_SHOW_FLYTHROUGH:
			ShowFlythroughWindow();
			return true;
		case IDV_SHOW_EXPLORE3D:
			ShowExplore3DWindow();
			return true;
		case IDV_CAPTURE_SCREEN:
			return CaptureScreen(uparams.at(0));
		case IDV_FLYTHROUGH_RECORD_START:
			m_guiView.SetIsRecordingOn(true);
			return true;
		case IDV_FLYTHROUGH_RECORD_STOP:
			m_guiView.SetIsRecordingOn(false);
			return true;
		default:
			return true;
		}
	}
	catch (const COMException& e)
	{
		ErrorHandler::Log(e);
	}
	catch (const TRException& e)
	{
		ErrorHandler::Log(e);
	}
	catch (const std::exception& e)
	{
		ErrorHandler::Log(e);
	}
	catch (...)
	{
		ErrorHandler::Log("Unknown Exception: No details available");
	}
	return false;
}


bool TerrainView::CaptureScreen(unsigned frameNum)
{
	try
	{
		THROW_TREXCEPTION_IF_FAILED(!m_outputDirectoryPath.empty(), L"Failed to capture screen because the output directory was not choose");
		m_d3dView.CaptureScreen(m_outputDirectoryPath, frameNum);
		return true;
	}
	catch (const COMException& e)
	{
		ErrorHandler::Log(e);
	}
	catch (const TRException& e)
	{
		ErrorHandler::Log(e);
	}
	catch (const std::exception& e)
	{
		ErrorHandler::Log(e);
	}
	catch (...)
	{
		ErrorHandler::Log("Unknown Exception: No details available");
	}
	return false;
}

void TerrainView::SetOutputDirectory(const std::wstring& outputDirectoryPath)
{
	m_outputDirectoryPath = outputDirectoryPath;
	m_guiView.SetOutputDirectory(m_outputDirectoryPath);
}
bool TerrainView::Render()
{
	bool bresult;
	m_d3dView.BeginScene(m_backgroungColor.x, m_backgroungColor.y, m_backgroungColor.z, 1.0f);
	m_guiView.BeginFrame();

	m_terrainModel->Render(m_d3dView.GetDeviceContext().Get());

	m_guiView.DisplayWindows();
	
	m_guiView.EndFrame();
	m_d3dView.EndScene();

	return true;
}

Microsoft::WRL::ComPtr<ID3D11Device> TerrainView::GetDevice()
{
	return m_d3dView.GetDevice();
}
Microsoft::WRL::ComPtr<ID3D11DeviceContext> TerrainView::GetDeviceContext()
{
	return m_d3dView.GetDeviceContext();
}


void TerrainView::SetController(IControllerPtr terrainController)
{
	m_terrainController = terrainController;
}
void TerrainView::SetModel(IModelPtr terrainModel)
{
	m_terrainModel = terrainModel;
}

void TerrainView::HandleIModelState(const MeshGroupState& state) {
	m_guiView.HandleIModelState(state);
}

void TerrainView::HandleIModelState(const FlythroughState& state)
{
	m_guiView.HandleIModelState(state);
}
void TerrainView::HandleIModelState(const Explore3DState& state) 
{
	m_guiView.HandleIModelState(state);
}
void TerrainView::HandleIModelState(const GeneralModelState& state)
{
	m_guiView.HandleIModelState(state);
}

void TerrainView::ShowHelp()
{
	m_guiView.ShowHelp();
}
void TerrainView::ShowGeneralWindow()
{
	m_guiView.ShowGeneralWindow();
}
void TerrainView::ShowExplore3DWindow()
{
	m_guiView.ShowExplore3DWindow();
}
void TerrainView::ShowFlythroughWindow()
{
	m_guiView.ShowFlythroughWindow();
}
