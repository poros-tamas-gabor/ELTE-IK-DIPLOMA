#include "GuiController.h"
#include <algorithm>
#include "../Model/TerrainModel.h"
#include "../resource.h"
#include "MessageSystem.h"
#include <commdlg.h>
#include "Tasks.h"
#include <memory>
#include <CommCtrl.h>
#include <atomic>
#include "../ProgressBar.h"

GuiController::GuiController()
{
	m_handledMsgs.push_back(IDMENU_FIlE_TERRAIN_SHARP);
	m_handledMsgs.push_back(IDMENU_FIlE_TERRAIN_PROJECT_SHARP);
	m_handledMsgs.push_back(IDMENU_FIlE_TERRAIN_SOFT);
	m_handledMsgs.push_back(IDMENU_FIlE_TERRAIN_PROJECT_SOFT);
	m_handledMsgs.push_back(IDMENU_FIlE_CAMERA_TRAJECTORY);
	m_handledMsgs.push_back(IDMENU_FIlE_PARAMETERS);

	m_handledMsgs.push_back(IDC_SLIDER_CAMERA_SPEED);
	m_handledMsgs.push_back(IDC_SLIDER_CAMERA_ROTATION_SPEED);
	m_handledMsgs.push_back(IDC_BUTTON_CAMERA_RESET);
	m_handledMsgs.push_back(IDC_SLIDER_PROJECTION_FIELD_OF_VIEW);
	m_handledMsgs.push_back(IDC_SLIDER_PROJECTION_NEAR_SCREEN);
	m_handledMsgs.push_back(IDC_SLIDER_PROJECTION_FAR_SCREEN);

	m_handledMsgs.push_back(IDC_BUTTON_FLYTHROUGH_MODE);
	m_handledMsgs.push_back(IDC_BUTTON_3DEXPLORE_MODE);
	m_handledMsgs.push_back(IDC_BUTTON_FLYTHROUGH_START);
	m_handledMsgs.push_back(IDC_BUTTON_FLYTHROUGH_PAUSE);
	m_handledMsgs.push_back(IDC_BUTTON_FLYTHROUGH_PAUSE);
	m_handledMsgs.push_back(IDC_BUTTON_FLYTHROUGH_STOP);
	m_handledMsgs.push_back(IDC_BUTTON_FLYTHROUGH_RECORD);
	m_handledMsgs.push_back(IDC_SLIDER_FLYTHROUGH_SPEED);

	m_handledMsgs.push_back(IDC_SLIDER_IRENDERABLE_SCALE);
	m_handledMsgs.push_back(IDC_SLIDER_IRENDERABLE_ROTATION);
	m_handledMsgs.push_back(IDC_SLIDER_IRENDERABLE_TRANSLATION);
	m_handledMsgs.push_back(IDC_SLIDER_IRENDERABLE_COLOR);
	m_handledMsgs.push_back(IDC_BUTTON_CLEAR_MESHES);
	m_handledMsgs.push_back(IDC_BUTTON_CLEAR_TRAJECTORY);
	m_handledMsgs.push_back(IDC_CHECKBOX_IRENDERABLE_ISSEEN);

	m_handledMsgs.push_back(IDC_SLIDER_FLYTHROUGH_FRAME);
}
GuiController::~GuiController() {}

void GuiController::SetMessageSystem(ControllerMessageSystemPtr messageSystem)
{
	m_messageSystem = messageSystem;
}

bool GuiController::CanHandle(unsigned int message) const
{
	auto it = std::find(m_handledMsgs.begin(), m_handledMsgs.end(), message);
	return it != m_handledMsgs.end();
}


bool GuiController::IsActive() const {
	return m_isActive;
}

void GuiController::OpenFileDialog(wchar_t* filePath, unsigned buffer)
{
	//wchar_t filePath[260];      // buffer for file name
	OPENFILENAME ofn;			// common dialog box structure
	HWND hwnd = NULL;           // owner window
	HANDLE hf;					// file handle

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
	ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
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

void GuiController::OpenFileDialogMultipleSelection(std::vector<std::wstring>& files)
{
	wchar_t filePath[260];      // buffer for file name
	OPENFILENAME ofn;			// common dialog box structure
	HWND hwnd = NULL;           // owner window
	HANDLE hf;					// file handle

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
	ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
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
void GuiController::HandleMessage(unsigned int message, float* fparam, unsigned* uparam)
{
	switch (message)
	{
	case IDMENU_FIlE_TERRAIN_SHARP:
	{
		wchar_t filePath[260];
		this->OpenFileDialog(filePath, 260);
		if(!std::wstring(filePath).empty())
			this->m_terrainModel->LoadTerrainSharpEdges(filePath);
		break;
	}
	case IDMENU_FIlE_TERRAIN_SOFT:
	{
		wchar_t filePath[260];
		this->OpenFileDialog(filePath, 260);
		std::wstring filepathwstr(filePath);

		if (!filepathwstr.empty())
		{

			bool loading(true);
			ICallablePtr task = std::make_shared<Task_LoadTerrainSoft>(filepathwstr,this->m_terrainModel);
			std::thread worker(std::ref(*task));

			ProgressBar pb(loading, worker);

			pb.Run();
			//HWND dialogWnd = CreateWindowEx(0, WC_DIALOG, L"Proccessing...", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			//	600, 300, 280, 120, NULL, NULL, NULL, NULL);
			//HWND pBarWnd = CreateWindowEx(NULL, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | PBS_MARQUEE, 40, 20, 200, 20,
			//	dialogWnd, (HMENU)1, NULL, NULL);
			//
			//MSG msg;

			//PostMessage(pBarWnd, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
			//PostMessage(pBarWnd, PBM_SETPOS, 0, 0);
			//while (GetMessage(&msg, NULL, NULL, NULL))
			//{
			//	if (!loading)
			//	{
			//		DestroyWindow(dialogWnd);
			//	}
			//
			//	if (worker.joinable())
			//	{
			//		worker.join();
			//		loading = false;
			//	}
			//}


		}
		break;
	}
	case IDMENU_FIlE_CAMERA_TRAJECTORY:
	{
		wchar_t filePath[260];
		this->OpenFileDialog(filePath, 260);
		if (!std::wstring(filePath).empty())
			this->m_terrainModel->LoadCameraTrajectory(filePath);
		break;
	}
	case IDMENU_FIlE_TERRAIN_PROJECT_SHARP:
	{
		std::vector<std::wstring> files;
		this->OpenFileDialogMultipleSelection(files);

		for (auto& file : files)
		{
			OutputDebugStringW(file.c_str());
		}
		this->m_terrainModel->LoadTerrainSharpEdges_Project(files);
		break;
	}

	case IDMENU_FIlE_TERRAIN_PROJECT_SOFT:
	{
		std::vector<std::wstring> files;
		this->OpenFileDialogMultipleSelection(files);

		for (auto& file : files)
		{
			//ICallablePtr task = std::make_shared<Task_LoadTerrainSoft>()
			//OutputDebugStringW(file.c_str());
		}
		//this->m_terrainModel->LoadTerrainSoftEdges_Project(files);
		break;
	}
	case IDMENU_FIlE_PARAMETERS:
	{
		wchar_t filePath[260];
		this->OpenFileDialog(filePath, 260);
		if (!std::wstring(filePath).empty())
			this->m_terrainModel->LoadParameters(filePath);
		break;
	}


	case IDC_SLIDER_CAMERA_SPEED:
	{
		this->m_terrainModel->UpdateCameraProperties(IDM_SET_CAMERA_SPEED, *fparam);
		break;
	}
	case IDC_SLIDER_CAMERA_ROTATION_SPEED:
	{
		this->m_terrainModel->UpdateCameraProperties(IDM_SET_CAMERA_ROTATION_SPEED, *fparam);
		break;
	}
	case IDC_BUTTON_CAMERA_RESET:
	{
		this->m_terrainModel->ResetCamera();
		break;
	}

	case IDC_SLIDER_PROJECTION_FIELD_OF_VIEW:
	{
		this->m_terrainModel->UpdateCameraProperties(IDM_SET_CAMERA_FIELD_OF_VIEW, *fparam);
		break;
	}
	case IDC_SLIDER_PROJECTION_NEAR_SCREEN:
	{
		this->m_terrainModel->UpdateCameraProperties(IDM_SET_CAMERA_ASPECT_NEAR_SCREEN, *fparam);
		break;
	}
	case IDC_SLIDER_PROJECTION_FAR_SCREEN:
	{
		this->m_terrainModel->UpdateCameraProperties(IDM_SET_CAMERA_ASPECT_FAR_SCREEN, *fparam);
		break;
	}
	case IDC_BUTTON_FLYTHROUGH_MODE:
	{
		if (this->m_terrainModel->IsTrajectoryInitialized())
		{
			this->m_messageSystem->Publish(IDCC_ACTIVATE_FLYTHROUGH, NULL, NULL);
		}
		break;
	}
	case IDC_BUTTON_3DEXPLORE_MODE:
	{
		this->m_messageSystem->Publish(IDCC_ACTIVATE_3DEXPLORE, NULL, NULL);
		break;

	}
	case IDC_BUTTON_FLYTHROUGH_START:
	{
		this->m_messageSystem->Publish(IDCC_START_FLYTHROUGH, NULL, NULL);
		break;
	}
	case IDC_BUTTON_FLYTHROUGH_PAUSE:
	{
		this->m_messageSystem->Publish(IDCC_PAUSE_FLYTHROUGH ,NULL, NULL);
		break;
	}
	case IDC_BUTTON_FLYTHROUGH_STOP:
	{
		this->m_messageSystem->Publish(IDCC_STOP_FLYTHROUGH, NULL, NULL);
		break;
	}
	case IDC_SLIDER_FLYTHROUGH_FRAME:
	{
		this->m_messageSystem->Publish(IDCC_STOP_FLYTHROUGH, fparam, uparam);
		break;
	}
	case IDC_BUTTON_FLYTHROUGH_RECORD: 
	{
		this->m_terrainView->CaptureScreen();
		break;
	}
	case IDC_SLIDER_FLYTHROUGH_SPEED: 
	{
		this->m_messageSystem->Publish(IDCC_SPEED_FLYTHROUGH, fparam, uparam);
		break;
	}
	case IDC_CHECKBOX_IRENDERABLE_ISSEEN:
	{
		this->m_terrainModel->TransformIRenderable(IDM_IRENDERABLE_ISSEEN, *uparam, fparam);
		break;
	}

	case IDC_SLIDER_IRENDERABLE_SCALE: 		 
	{
		this->m_terrainModel->TransformIRenderable(IDM_TRANSFORMATION_IRENDERABLE_SCALE, *uparam, fparam);
		break;
	}
	case IDC_SLIDER_IRENDERABLE_ROTATION:	 
	{
		this->m_terrainModel->TransformIRenderable(IDM_TRANSFORMATION_IRENDERABLE_ROTATION, *uparam, fparam);
		break; 
	}
	case IDC_SLIDER_IRENDERABLE_TRANSLATION:
	{
		this->m_terrainModel->TransformIRenderable(IDM_TRANSFORMATION_IRENDERABLE_TRANSLATION, *uparam, fparam);
		break;
	}
	case IDC_SLIDER_IRENDERABLE_COLOR:
	{
		this->m_terrainModel->TransformIRenderable(IDM_TRANSFORMATION_IRENDERABLE_COLOR, *uparam, fparam);
		break;
	}
	case IDC_BUTTON_CLEAR_MESHES:
	{
		this->m_terrainModel->ClearTerrain();
		break;
	}
	case IDC_BUTTON_CLEAR_TRAJECTORY:
	{
		this->m_terrainModel->ClearCameraTrajectory();
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

void GuiController::SetHWND(HWND hwnd)
{
	m_hwnd = hwnd;
}