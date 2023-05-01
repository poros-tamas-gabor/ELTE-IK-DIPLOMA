#include "GuiController.h"
#include <algorithm>
#include "../Model/TerrainModel.h"
#include "../resource.h"
#include "MessageSystem.h"
#include "../ProgressBar.h"
#include "Tasks.h"
#include "../DialogWin.h"


GuiController::GuiController()
{
}
GuiController::~GuiController() {}

void GuiController::SetMessageSystem(ControllerMessageSystemPtr messageSystem)
{
	m_messageSystem = messageSystem;
}

bool GuiController::IsActive() const {
	return m_isActive;
}

void GuiController::StartWorkerThread(const ICallableCreator& creator, std::atomic_bool& running)
{
	ICallablePtr process = creator.Create();
	std::thread worker(std::ref(*process));
	ProgressBar pb(running);
	if (worker.joinable())
	{
		pb.Run();
		worker.join();
	}
}

bool GuiController::HandleMessage(IControllerMessageIDs message, const std::vector<float>& fparam, const std::vector<unsigned>& uparam)
{
	IModelMessageIDs modelmsg = IDC2IDM(message);
	switch (message)
	{

	case IDMENU_FIlE_TERRAIN_SHARP:
	{
		wchar_t filePath[260];
		OpenFileDialog(filePath, 260, m_filter_stl);
		std::wstring filepathwstr(filePath);
		if (!filepathwstr.empty())
			return m_terrainModel->HandleMessage(modelmsg, { filepathwstr }, fparam, uparam);
		return false;
	}
	case IDMENU_FIlE_TERRAIN_SOFT:
	{
		wchar_t filePath[260];
		OpenFileDialog(filePath, 260, m_filter_stl);

		std::wstring filepathwstr(filePath);
		if (!filepathwstr.empty())
			return m_terrainModel->HandleMessage(modelmsg, { filepathwstr }, fparam, uparam);
		return false;
	}
	case IDMENU_FIlE_CAMERA_TRAJECTORY:
	{
		wchar_t filePath[260];
		OpenFileDialog(filePath, 260, m_filter_csv);
		std::wstring filepathwstr(filePath);
		if (!filepathwstr.empty())
			return m_terrainModel->HandleMessage(modelmsg, { filepathwstr }, fparam, uparam);
		return false;
	}
	case IDMENU_FIlE_TERRAIN_PROJECT_SHARP:
	{
		std::vector<std::wstring> files;
		OpenFileDialogMultipleSelection(files, m_filter_stl);
		return m_terrainModel->HandleMessage(modelmsg, files, fparam, uparam);
	}

	case IDMENU_FIlE_TERRAIN_PROJECT_SOFT:
	{
		std::vector<std::wstring> files;
		OpenFileDialogMultipleSelection(files, m_filter_stl);
		return m_terrainModel->HandleMessage(modelmsg, files, fparam, uparam);
	}
	case IDMENU_FIlE_CONFIGURATION:
	{
		wchar_t filePath[260];
		OpenFileDialog(filePath, 260, m_filter_json);
		std::wstring filepathwstr(filePath);
		if (!filepathwstr.empty())
			return m_terrainModel->HandleMessage(modelmsg, { filepathwstr }, fparam, uparam);
		return false;
	}

	case IDMENU_HELP:
	{
		m_terrainView->ShowHelp();
		return true;
	}

	case IDMENU_WINDOWS_EXPLORE3D:
	{
		bool isFlythroughModeOn;
		isFlythroughModeOn = m_messageSystem->Publish(IDCC_IS_FLYTHROUGH_MODE_ON, {}, {});
		if (!isFlythroughModeOn)
			m_terrainView->ShowExplore3DWindow();
		return !isFlythroughModeOn;
	}
	case IDMENU_WINDOWS_FLYTHROUGH:
	{
		bool isFlythroughModeOn;
		isFlythroughModeOn = m_messageSystem->Publish(IDCC_IS_FLYTHROUGH_MODE_ON, {}, {});
		if(isFlythroughModeOn)
			m_terrainView->ShowFlythroughWindow();
		return isFlythroughModeOn;
	}
	case IDMENU_WINDOWS_GENERAL:
	{
		m_terrainView->ShowGeneralWindow();
		return true;
	}
	case IDMENU_FILE_OUTPUT_DIRECTORY:
	{
		std::wstring dir;
		OpenFileDialogDirectory(dir);
		m_terrainView->SetOutputDirectory(dir);
		return true;
	}
	case IDC_BUTTON_CLEAR_TRAJECTORY:
	{
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

void GuiController::SetTerrainView(IViewPtr pView)
{
	m_terrainView = pView;
}
void GuiController::SetTerrainModel(IModelPtr pModel)
{
	m_terrainModel = pModel;
}
void GuiController::SetMouse(MousePtr mouse) {}

void GuiController::SetKeyboard(KeyboardPtr keyboard) {}


bool  GuiController::Initialize(IModelPtr pModel, IViewPtr pView, MousePtr mouse, KeyboardPtr keyboard)
{
	if (pModel.get() == nullptr || mouse.get() == nullptr || keyboard.get() == nullptr)
	{
		return false;
	}
	SetTerrainModel(pModel);
	SetMouse(mouse);
	SetKeyboard(keyboard);
	SetTerrainView(pView);
	return true;
}
void GuiController::Shutdown() {}

void GuiController::HandleIModelState(const MeshGroupState&) {}
void GuiController::HandleIModelState(const FlythroughState&) {}
void GuiController::HandleIModelState(const Explore3DState&) {}
void GuiController::HandleIModelState(const GeneralModelState&) {}