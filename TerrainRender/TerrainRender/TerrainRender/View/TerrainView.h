#ifndef TERRAIN_VIEW_H
#define TERRAIN_VIEW_H

///////////////////////////////////////////////////////////////////////////////
// TerrainView.h
// =============
// A derived class of the IView interface, responsible for aggregating other views and displaying the overall content
// on the screen. Receives instructions from the controller and updates the displayed image through the D3DView.
//
// The TerrainView class is responsible for creating and managing the other views that make up the user interface,
// including the D3DView and GuiView.
//
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

#include <DirectXMath.h>
#include "IView.h"
#include <vector>
#include <memory>
#include "D3DView.h"
#include "GuiView.h"
#include "../Controller/IController.h"
#include "../Model/IModel.h"
#include <wrl/client.h>

class IModel;

class TerrainView : public IView
{
private:
	D3DView				m_d3dView;
	GuiView				m_guiView;

	IModelPtr			m_terrainModel;
	IControllerPtr		m_terrainController;

	FlythroughState		m_flythroughState;
	std::wstring		m_outputDirectoryPath;

	Vector3D			m_backgroungColor = { 88.0f/255,  114.0f / 255,  127.0f / 255 };
public:

	TerrainView();
	TerrainView(const TerrainView&) = delete;
	~TerrainView();
	bool Initalize(HWND hwnd, float screenWidth, float screenHeight, bool fullscreen = false, bool vsync = true);
	bool Resize(unsigned screenWidth, unsigned screenHeight);
	void Shutdown();
	bool RenderFrame();
	bool HandleMessage(IViewMessageIDs message, const std::vector<std::wstring>& stringParams, const std::vector<float>& fparams, const std::vector<unsigned>& uparams) override;

	void SetController(IControllerPtr terrainController);
	void SetModel(IModelPtr terrainModel);

	void HandleIModelState(const MeshGroupState&) override;
	void HandleIModelState(const FlythroughState&) override;
	void HandleIModelState(const Explore3DState&) override;
	void HandleIModelState(const GeneralModelState&) override;

	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice();
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetDeviceContext();

private:
	bool Render();
	void SetOutputDirectory(const std::wstring& m_outputDirectoryPath);
	bool CaptureScreen(unsigned frameNum);
	void ShowHelp();
	void ShowGeneralWindow();
	void ShowExplore3DWindow();
	void ShowFlythroughWindow();
};

typedef std::shared_ptr<TerrainView> TerrainViewPtr;

#endif