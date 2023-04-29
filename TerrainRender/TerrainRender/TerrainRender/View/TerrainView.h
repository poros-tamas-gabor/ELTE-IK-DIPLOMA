#ifndef TERRAIN_VIEW_H
#define TERRAIN_VIEW_H
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
	D3DView					m_d3dView;
	GuiView					m_guiView;

	IModelPtr				m_terrainModel;
	IControllerPtr			m_terrainController;

	FlythroughState			m_flythroughState;
	std::wstring			m_outputDirectoryPath;

	Vector3D				m_backgroungColor = { 88.0f/255,  114.0f / 255,  127.0f / 255 };

	bool			Render();
public:

	TerrainView();
	TerrainView(const TerrainView&) = delete;
	~TerrainView();
	bool Initalize(HWND hwnd, float screenWidth, float screenHeight, bool fullscreen = false, bool vsync = true);
	bool Resize(unsigned screenWidth, unsigned screenHeight);
	void Shutdown();
	bool RenderFrame();
	bool CaptureScreen(unsigned frameNum) override;
	void SetOutputDirectory(const std::wstring& m_outputDirectoryPath) override;

	void SetController(IControllerPtr terrainController);
	void SetModel(IModelPtr terrainModel);

	void HandleIModelState(const MeshGroupState&) override;
	void HandleIModelState(const FlythroughState&) override;
	void HandleIModelState(const Explore3DState&) override;
	void HandleIModelState(const CameraState&) override;
	void ShowHelp() override;	
	void ShowGeneralWindow()   override;
	void ShowExplore3DWindow() override;
	void ShowFlythroughWindow()override; 


	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice();
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetDeviceContext();


};
typedef std::shared_ptr<TerrainView> TerrainViewPtr;

#endif