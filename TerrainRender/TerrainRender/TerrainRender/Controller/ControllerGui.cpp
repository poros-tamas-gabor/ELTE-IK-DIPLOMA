#include "ControllerGui.h"
#include <algorithm>
#include "../Model/TerrainModel.h"
#include "../resource.h"
#include "MessageSystem.h"
#include "../ProgressBar.h"
#include "../DialogWin.h"
#include "../ErrorHandler.h"


ControllerGui::ControllerGui() {}
ControllerGui::~ControllerGui() {}

bool ControllerGui::IsActive() const {
	return m_isActive;
}

bool  ControllerGui::Initialize(IModelPtr pModel, IViewPtr pView, MousePtr mouse, KeyboardPtr keyboard)
{
	if (pModel == nullptr || mouse == nullptr || keyboard == nullptr || pView == nullptr)
	{
		return false;
	}
	SetTerrainModel(pModel);
	SetMouse(mouse);
	SetKeyboard(keyboard);
	SetTerrainView(pView);
	return true;
}
void ControllerGui::Shutdown() {}

bool ControllerGui::HandleMessage(IControllerMessageIDs message, const std::vector<float>& fparam, const std::vector<unsigned>& uparam)
{
	try {
		IModelMessageIDs modelmsg = IDC2IDM(message);
		switch (message)
		{
		case IDMENU_FIlE_TERRAIN_SHARP:
		case IDMENU_FIlE_TERRAIN_SOFT:
		case IDMENU_FIlE_CAMERA_TRAJECTORY:
		case IDMENU_FIlE_CONFIGURATION:
		{
			const wchar_t* filter = nullptr;
			if (message == IDMENU_FIlE_TERRAIN_SHARP || message == IDMENU_FIlE_TERRAIN_SOFT)
				filter = m_filter_stl;
			else if (message == IDMENU_FIlE_CAMERA_TRAJECTORY)
				filter = m_filter_csv;
			else if (message == IDMENU_FIlE_CONFIGURATION)
				filter = m_filter_json;
			std::wstring filePath;
			OpenFileDialog(filePath, filter);
			if (!filePath.empty())
			{
				LoadFiles(modelmsg, { filePath }, fparam, uparam);
				return true;
			}
			return false;
		}
		case IDMENU_FIlE_TERRAIN_PROJECT_SHARP:
		case IDMENU_FIlE_TERRAIN_PROJECT_SOFT:
		{
			std::vector<std::wstring> files;
			OpenFileDialogMultipleSelection(files, m_filter_stl);
			LoadFiles(modelmsg, files, fparam, uparam);
			return true;
		}

		case IDMENU_HELP:
		{
			m_terrainView->HandleMessage(IDC2IDV(IDMENU_HELP), {}, {}, {}); //ShowHelp();
			return true;
		}

		case IDMENU_WINDOWS_EXPLORE3D:
		{
			bool isFlythroughModeOn;
			THROW_TREXCEPTION_IF_FAILED((m_messageSystem != nullptr), L"Controller message system is not initialized");
			isFlythroughModeOn = m_messageSystem->Publish(IDCC_IS_FLYTHROUGH_MODE_ON, {}, {});
			if (!isFlythroughModeOn)
				m_terrainView->HandleMessage(IDC2IDV(IDMENU_WINDOWS_EXPLORE3D), {}, {}, {});
			return !isFlythroughModeOn;
		}
		case IDMENU_WINDOWS_FLYTHROUGH:
		{
			bool isFlythroughModeOn;
			THROW_TREXCEPTION_IF_FAILED((m_messageSystem != nullptr), L"Controller message system is not initialized");
			isFlythroughModeOn = m_messageSystem->Publish(IDCC_IS_FLYTHROUGH_MODE_ON, {}, {});
			if (isFlythroughModeOn)
				m_terrainView->HandleMessage(IDC2IDV(IDMENU_WINDOWS_FLYTHROUGH), {}, {}, {});
			return isFlythroughModeOn;
		}
		case IDMENU_WINDOWS_GENERAL:
		{
			m_terrainView->HandleMessage(IDC2IDV(IDMENU_WINDOWS_GENERAL), {}, {}, {});
			return true;
		}
		case IDMENU_FILE_OUTPUT_DIRECTORY:
		{
			std::wstring dir;
			OpenFileDialogDirectory(dir);
			if (!dir.empty())
			{
				m_terrainView->HandleMessage(IDC2IDV(IDMENU_FILE_OUTPUT_DIRECTORY), { dir }, {}, {});
				THROW_TREXCEPTION_IF_FAILED((m_messageSystem != nullptr), L"Controller message system is not initialized");
				m_messageSystem->Publish(IDCC_OUTPUT_DIR_CHOOSED, {}, {});
			}
			return true;
		}
		case IDC_BUTTON_CLEAR_TRAJECTORY:
		{
			THROW_TREXCEPTION_IF_FAILED((m_messageSystem != nullptr), L"Controller message system is not initialized");
			m_messageSystem->Publish(IDC_ACTIVATE_3DEXPLORE_MODE, fparam, uparam);
			return m_terrainModel->HandleMessage(modelmsg, {}, fparam, uparam);
		}
		case IDC_SET_CAMERA_FIELD_OF_VIEW:
		case IDC_SET_CAMERA_NEAR_SCREEN:
		case IDC_SET_CAMERA_FAR_SCREEN:
		case IDC_MESH_SET_ISSEEN:
		case IDC_MESH_SET_COLOR:
		case IDC_MESH_GROUP_SCALE:
		case IDC_MESH_GROUP_ROTATION:
		case IDC_MESH_GROUP_TRANSLATION:
		case IDC_TRAJECTORY_SET_ISSEEN:
		case IDC_TRAJECTORY_ROTATION:
		case IDC_TRAJECTORY_TRANSLATION:
		case IDC_BUTTON_CLEAR_MESHES:
		case IDC_SET_TIME_E3D:
		case IDC_SET_START_TIME_TRAJECTORY:
		case IDC_ORIGO_SET_LONGITUDE:
		case IDC_ORIGO_SET_LATITUDE:
		case IDC_XZ_PLANE_GRID_SET_ISSEEN:
		case IDC_PIXELSHADER_SET_SHADING:
		{
			return m_terrainModel->HandleMessage(modelmsg, {}, fparam, uparam);
		}
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

void ControllerGui::SetMessageSystem(ControllerMessageSystemPtr messageSystem)
{
	m_messageSystem = messageSystem;
}
void ControllerGui::SetTerrainView(IViewPtr pView)
{
	m_terrainView = pView;
}
void ControllerGui::SetTerrainModel(IModelPtr pModel)
{
	m_terrainModel = pModel;
}
void ControllerGui::SetMouse(MousePtr mouse) {}

void ControllerGui::SetKeyboard(KeyboardPtr keyboard) {}


void ControllerGui::HandleIModelState(const MeshGroupState&) {}
void ControllerGui::HandleIModelState(const FlythroughState&) {}
void ControllerGui::HandleIModelState(const Explore3DState&) {}
void ControllerGui::HandleIModelState(const GeneralModelState&) {}

void ControllerGui::LoadFiles(IModelMessageIDs message, const std::vector<std::wstring>& sparam, const std::vector<float>& fparam, const std::vector<unsigned>& uparam)
{
	std::thread workerThread([=] {
		m_terrainModel->HandleMessage(message, sparam, fparam, uparam);
		});

#ifndef _GTEST
	std::atomic_bool running = true;
	std::shared_ptr<ProgressBar> pb = std::make_shared<ProgressBar>(running);
	std::thread pbThread([=] {pb->Run(); });
#endif // !_GTEST


	if (workerThread.joinable())
	{
		workerThread.join();
#ifndef _GTEST
		running = false;
		pbThread.join();
#endif // !_GTEST

	}
}