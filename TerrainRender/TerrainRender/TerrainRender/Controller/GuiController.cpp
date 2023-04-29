#include "GuiController.h"
#include <algorithm>
#include "../Model/TerrainModel.h"
#include "../resource.h"
#include "MessageSystem.h"
#include <commdlg.h>
#include "../ProgressBar.h"
#include "Tasks.h"
#include <shlobj_core.h>

GuiController::GuiController()
{
	m_handledMsgs.push_back(IDMENU_FIlE_TERRAIN_SHARP);
	m_handledMsgs.push_back(IDMENU_FIlE_TERRAIN_PROJECT_SHARP);
	m_handledMsgs.push_back(IDMENU_FIlE_TERRAIN_SOFT);
	m_handledMsgs.push_back(IDMENU_FIlE_TERRAIN_PROJECT_SOFT);
	m_handledMsgs.push_back(IDMENU_FIlE_CAMERA_TRAJECTORY);
	m_handledMsgs.push_back(IDMENU_FIlE_CONFIGURATION);
	m_handledMsgs.push_back(IDMENU_FILE_OUTPUT_DIRECTORY);
	m_handledMsgs.push_back(IDMENU_HELP);


	m_handledMsgs.push_back(IDC_SLIDER_PROJECTION_FIELD_OF_VIEW);
	m_handledMsgs.push_back(IDC_SLIDER_PROJECTION_NEAR_SCREEN);
	m_handledMsgs.push_back(IDC_SLIDER_PROJECTION_FAR_SCREEN);

	m_handledMsgs.push_back(IDC_ACTIVATE_FLYTHROUGH_MODE);
	m_handledMsgs.push_back(IDC_ACTIVATE_3DEXPLORE_MODE);


	m_handledMsgs.push_back(IDC_MESH_GROUP_SCALE);
	m_handledMsgs.push_back(IDC_MESH_GROUP_ROTATION);
	m_handledMsgs.push_back(IDC_MESH_GROUP_TRANSLATION);
	m_handledMsgs.push_back(IDC_MESH_SET_COLOR);
	m_handledMsgs.push_back(IDC_BUTTON_CLEAR_MESHES);
	m_handledMsgs.push_back(IDC_BUTTON_CLEAR_TRAJECTORY);
	m_handledMsgs.push_back(IDC_IRENDERABLE_SET_ISSEEN);

	m_handledMsgs.push_back(IDC_INPUT_3DE_UNIXTIME);
	m_handledMsgs.push_back(IDC_INPUT_FLYTHROUGH_UNIXTIME);

	m_handledMsgs.push_back(IDMENU_WINDOWS_EXPLORE3D);
	m_handledMsgs.push_back(IDMENU_WINDOWS_FLYTHROUGH);
	m_handledMsgs.push_back(IDMENU_WINDOWS_GENERAL);
}
GuiController::~GuiController() {}

void GuiController::SetMessageSystem(ControllerMessageSystemPtr messageSystem)
{
	m_messageSystem = messageSystem;
}

bool GuiController::CanHandle(IControllerMessageIDs message) const
{
	auto it = std::find(m_handledMsgs.begin(), m_handledMsgs.end(), message);
	return it != m_handledMsgs.end();
}


bool GuiController::IsActive() const {
	return m_isActive;
}

bool GuiController::IsFlythroughModeOn(void) const
{
	return false;
}

void GuiController::OpenFileDialog(wchar_t* filePath, unsigned buffer, const wchar_t* filter)
{
	//wchar_t filePath[260];      // buffer for file name
	OPENFILENAME ofn;			// common dialog box structure
	HWND hwnd = NULL;           // owner window

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = filePath;
	//
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	//
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(wchar_t) * buffer;
	ofn.lpstrFilter = filter;//fileFilter.c_str();
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; 

	// Display the Open dialog box. 
	if (GetOpenFileName(&ofn) == TRUE)
	{
		OutputDebugStringW(filePath);
	}
}

void GuiController::OpenFileDialogDirectory(std::wstring& directory)
{
	BROWSEINFO bi = { 0 };
	bi.lpszTitle = L"Select a folder";
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl != 0)
	{
		// get the name of the folder
		TCHAR path[MAX_PATH];
		if (SHGetPathFromIDList(pidl, path))
		{
			directory = path;
		}
		// free memory used
		IMalloc* imalloc = 0;
		if (SUCCEEDED(SHGetMalloc(&imalloc)))
		{
			imalloc->Free(pidl);
			imalloc->Release();
		}
	}
}

void GuiController::OpenFileDialogMultipleSelection(std::vector<std::wstring>& files, const wchar_t* filter)
{
	wchar_t filePath[260];      // buffer for file name
	OPENFILENAME ofn;			// common dialog box structure
	HWND hwnd = NULL;           // owner window

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = filePath;
	//
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	//
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(wchar_t) * 260;
	ofn.lpstrFilter = filter;//L"All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	// Display the Open dialog box. 
	if (GetOpenFileName(&ofn) == TRUE)
	{
		wchar_t* str = ofn.lpstrFile;
		std::wstring directory = str;
		str += (directory.length() + 1);
		
		//Only one element was selected
		if (*str == '\0')
			files.push_back(directory);
		while (*str) {
			std::wstring filename = str;
			str += (filename.length() + 1);
			files.push_back(directory + L"\\" + filename);
		}
	}
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


void GuiController::HandleMessage(IControllerMessageIDs message, const std::vector<float>& fparam, const std::vector<unsigned>& uparam)
{
	switch (message)
	{
	case IDMENU_FIlE_TERRAIN_SHARP:
	{
		wchar_t filePath[260];
		this->OpenFileDialog(filePath, 260, m_filter_stl);
		std::wstring filepathwstr(filePath);
		if (!filepathwstr.empty())
			m_terrainModel->HandleMessage(IDM_LOAD_TERRAIN_SHARP, { filepathwstr }, fparam, uparam);
		break;
	}
	case IDMENU_FIlE_TERRAIN_SOFT:
	{
		wchar_t filePath[260];
		this->OpenFileDialog(filePath, 260, m_filter_stl);

		std::wstring filepathwstr(filePath);
		if (!filepathwstr.empty())
			m_terrainModel->HandleMessage(IDM_LOAD_TERRAIN_SOFT, { filepathwstr }, fparam, uparam);
		break;
	}
	case IDMENU_FIlE_CAMERA_TRAJECTORY:
	{
		wchar_t filePath[260];
		this->OpenFileDialog(filePath, 260, m_filter_csv);
		std::wstring filepathwstr(filePath);
		if (!filepathwstr.empty())
			m_terrainModel->HandleMessage(IDM_LOAD_CAMERA_TRAJECTORY, { filepathwstr }, fparam, uparam);
		break;
	}
	case IDMENU_FIlE_TERRAIN_PROJECT_SHARP:
	{
		std::vector<std::wstring> files;
		this->OpenFileDialogMultipleSelection(files, m_filter_stl);
		m_terrainModel->HandleMessage(IDM_LOAD_PROJECT_SHARP, files, fparam, uparam);
		break;
	}

	case IDMENU_FIlE_TERRAIN_PROJECT_SOFT:
	{
		std::vector<std::wstring> files;
		this->OpenFileDialogMultipleSelection(files, m_filter_stl);
		m_terrainModel->HandleMessage(IDM_LOAD_PROJECT_SOFT, files, fparam, uparam);
		break;
	}
	case IDMENU_FIlE_CONFIGURATION:
	{
		wchar_t filePath[260];
		this->OpenFileDialog(filePath, 260, m_filter_json);
		std::wstring filepathwstr(filePath);
		if (!filepathwstr.empty())
			m_terrainModel->HandleMessage(IDM_LOAD_CONFIGURATION, { filepathwstr }, fparam, uparam);
		break;
	}

	case IDMENU_HELP:
	{
		this->m_terrainView->ShowHelp();
		break;
	}

	case IDMENU_WINDOWS_EXPLORE3D:
		m_terrainView->ShowExplore3DWindow();
		break;

	case IDMENU_WINDOWS_FLYTHROUGH:
		m_terrainView->ShowFlythroughWindow();
		break;

	case IDMENU_WINDOWS_GENERAL:
		this->m_terrainView->ShowGeneralWindow();
		break;

	case IDMENU_FILE_OUTPUT_DIRECTORY:
	{
		std::wstring dir;
		this->OpenFileDialogDirectory(dir);
		this->m_terrainView->SetOutputDirectory(dir);
		break;
	}

	case IDC_SLIDER_PROJECTION_FIELD_OF_VIEW:
	{
		this->m_terrainModel->HandleMessage(IDM_SET_CAMERA_FIELD_OF_VIEW, {}, fparam, uparam);
		break;
	}
	case IDC_SLIDER_PROJECTION_NEAR_SCREEN:
	{
		this->m_terrainModel->HandleMessage(IDM_SET_CAMERA_ASPECT_NEAR_SCREEN, {}, fparam, uparam);
		break;
	}
	case IDC_SLIDER_PROJECTION_FAR_SCREEN:
	{
		this->m_terrainModel->HandleMessage(IDM_SET_CAMERA_ASPECT_FAR_SCREEN, {}, fparam, uparam);
		break;
	}


	//case IDC_IRENDERABLE_SET_ISSEEN:
	//{
	//	this->m_terrainModel->TransformIRenderable(IDM_IRENDERABLE_ISSEEN, *uparam, fparam);
	//	break;
	//}
	//
	//case IDC_MESH_GROUP_SCALE: 		 
	//{
	//	this->m_terrainModel->TransformIRenderable(IDM_IRENDERABLE_SCALE, *uparam, fparam);
	//	break;
	//}
	//case IDC_MESH_GROUP_ROTATION:	 
	//{
	//	this->m_terrainModel->TransformIRenderable(IDM_IRENDERABLE_ROTATION, *uparam, fparam);
	//	break; 
	//}
	//case IDC_MESH_GROUP_TRANSLATION:
	//{
	//	this->m_terrainModel->TransformIRenderable(IDM_IRENDERABLE_TRANSLATION, *uparam, fparam);
	//	break;
	//}
	//case IDC_MESH_SET_COLOR:
	//{
	//	this->m_terrainModel->TransformIRenderable(IDM_IRENDERABLE_COLOR, *uparam, fparam);
	//	break;
	//}
	case IDC_BUTTON_CLEAR_MESHES:
	{
		this->m_terrainModel->HandleMessage(IDM_CLEAR_MESHES, {}, fparam, uparam);
		break;
	}
	case IDC_BUTTON_CLEAR_TRAJECTORY:
	{
		this->m_terrainModel->HandleMessage(IDM_CLEAR_TRAJECTORY, {}, fparam, uparam);
		this->m_messageSystem->Publish(IDC_ACTIVATE_3DEXPLORE_MODE, {}, {});
		break;
	}

	case IDC_INPUT_3DE_UNIXTIME:
	{
		this->m_terrainModel->HandleMessage(IDM_SET_TIME_E3D, {}, fparam, uparam);
		break;
	}
	case IDC_INPUT_FLYTHROUGH_UNIXTIME:
	{
		this->m_terrainModel->HandleMessage(IDM_SET_START_TIME_TRAJECTORY, {}, fparam, uparam);
		break;
	}
	}
}

void GuiController::SetTerrainView(IViewPtr pView)
{
	this->m_terrainView = pView;
}
void GuiController::SetTerrainModel(IModelPtr pModel)
{
	this->m_terrainModel = pModel;
}
void GuiController::SetMouse(MousePtr mouse) {}

void GuiController::SetKeyboard(KeyboardPtr keyboard) {}


bool  GuiController::Initialize(IModelPtr pModel, IViewPtr pView, MousePtr mouse, KeyboardPtr keyboard)
{
	if (pModel.get() == nullptr || mouse.get() == nullptr || keyboard.get() == nullptr)
	{
		return false;
	}
	this->SetTerrainModel(pModel);
	this->SetMouse(mouse);
	this->SetKeyboard(keyboard);
	this->SetTerrainView(pView);
	return true;
}
void GuiController::Shutdown() {}

void GuiController::HandleIModelState(const MeshGroupState&) {}
void GuiController::HandleIModelState(const FlythroughState&) {}
void GuiController::HandleIModelState(const Explore3DState&) {}
void GuiController::HandleIModelState(const CameraState&) {}