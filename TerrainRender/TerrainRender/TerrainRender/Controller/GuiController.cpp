#include "GuiController.h"
#include <algorithm>
#include "../Model/TerrainModel.h"
#include "../resource.h"


GuiController::GuiController()
{
	m_handledEvents.push_back(IDC_BUTTON_FIlE_TERRAIN);
	m_handledEvents.push_back(IDC_BUTTON_FIlE_CAMERA_TRAJECTORY);
	m_handledEvents.push_back(IDC_BUTTON_FIlE_CAMERA_PROPERTIES);
	m_handledEvents.push_back(IDC_SLIDER_CAMERA_SPEED);
	m_handledEvents.push_back(IDC_SLIDER_CAMERA_ROTATION_SPEED);
	m_handledEvents.push_back(IDC_BUTTON_CAMERA_RESET);
}
GuiController::~GuiController() {}

bool GuiController::CanHandle(unsigned int message) const
{
	auto it = std::find(m_handledEvents.begin(), m_handledEvents.end(), message);
	return it != m_handledEvents.end();
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
void GuiController::Control(unsigned int message, float* fparam, unsigned* uparam)
{
	switch (message)
	{
	case IDC_BUTTON_FIlE_TERRAIN:
	{
		wchar_t filePath[260];
		this->OpenFileDialog(filePath, 260);
		this->m_terrainModel->LoadTerrain(filePath);
		break;
	}
	case IDC_BUTTON_FIlE_CAMERA_TRAJECTORY:
	{
		wchar_t filePath[260];
		this->OpenFileDialog(filePath, 260);
		//this->m_terrainModel->LoadTerrain(filePath);
		break;
	}
	case IDC_BUTTON_FIlE_CAMERA_PROPERTIES:
	{
		wchar_t filePath[260];
		this->OpenFileDialog(filePath, 260);
		//this->m_terrainModel->LoadTerrain(filePath);
		break;
	}
	case IDC_SLIDER_CAMERA_SPEED:
	{
		this->m_terrainModel->UpdateCameraProperties(IDC_SET_CAMERA_SPEED, *fparam);
		break;
	}
	case IDC_SLIDER_CAMERA_ROTATION_SPEED:
	{
		this->m_terrainModel->UpdateCameraProperties(IDC_SET_CAMERA_ROTATION_SPEED, *fparam);
		break;
	}
	case IDC_BUTTON_CAMERA_RESET:
	{
		this->m_terrainModel->ResetCamera();
		break;
	}
	}
}
void GuiController::SetTerrainModel(TerrainModel* pModel)
{
	this->m_terrainModel = pModel;
}
void GuiController::SetMouse(Mouse* mouse) {}

void GuiController::SetKeyboard(Keyboard* keyboard) {}


bool GuiController::Initialize(TerrainModel* pModel, Mouse* mouse, Keyboard* keyboard) 
{
	if (pModel == nullptr)
		return false;
	SetTerrainModel(pModel);
	return true;
}
void GuiController::Shutdown() {}