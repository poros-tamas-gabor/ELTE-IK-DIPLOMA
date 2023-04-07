#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_dx11.h"
#include "../ImGui/imgui_impl_win32.h"
#include "../Controller/IController.h"
#include "IView.h"

class GuiView : public IView
{
private:
	struct IRenderableTransformation
	{
		unsigned	id;
		float		rotation[3]		= {0.0f,0.0f,0.0f};
		float		tranlation[3]	= { 0.0f,0.0f,0.0f };
		float		scaling			= 1.0f;
		float		color[4]		= { 1.0f,1.0f,1.0f,1.0f };
		bool		m_isSeen = true;
	};
private:
	IControllerPtr							m_terrainController;
	//ModelStates:
	std::vector<IRenderableTransformation>	m_TerrainTrasnformations;
	std::vector<IRenderableTransformation>	m_TrajectoryTransformation;
	std::vector<IRenderableState>			m_TerrainsState;
	FlythroughState							m_flythroughState;
	Explore3DState							m_explore3dState;
	int										m_frame;
public:
	bool Initalize(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, IControllerPtr controller);

	void BeginFrame();
	void EndFrame();
	void ShowSettingWindow();
	void MenuBar();
	void GeneralTab();
	void FlythroughTab();
	void Explore3DTab();
	void TerrainListBox();
	void IRenderablePopUp(unsigned int, IRenderableTransformation& t);

	void HandleIModelState(const std::vector<IRenderableState>&) override;
	void HandleIModelState(const FlythroughState&) override;
	void HandleIModelState(const Explore3DState&) override;
	std::vector<std::string> CollectTerrainIDNames(void);
	void Shutdown();
};