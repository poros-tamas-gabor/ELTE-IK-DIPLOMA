#ifndef GUI_VIEW_H
#define GUI_VIEW_H

///////////////////////////////////////////////////////////////////////////////
// GuiView.h
// ==========
// The GuiView class is responsible for displaying the graphical user interface (GUI) using components provided by the ImGui library.
// It initializes and manages components responsible for creating and handling the user interface, such as buttons, text fields, and lists.
// It also listens for events on the user interface, such as button clicks or text entered in a text field, and passes them on to the ControllerRouter.
//
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

#include "../win.h"
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_dx11.h"
#include "../ImGui/imgui_impl_win32.h"
#include "../Controller/IController.h"
#include "IView.h"
#include <wrl/client.h>

class GuiView : public IModelSubscriber
{
private:
	struct MeshGroupTransformation
	{
		float		rotation[3] = { 0.0f,0.0f,0.0f };
		float		tranlation[3] = { 0.0f,0.0f,0.0f };
		float		scaling = 1.0f;
		bool		m_isSeen = true;
	};
	struct MeshTransformation
	{
		unsigned	id;
		float		color[4]		= { 1.0f,1.0f,1.0f,1.0f };
		bool		m_isSeen = true;
	};
	struct TrajectoryTransformation
	{
		unsigned	id;
		float		rotation[3] = { 0.0f,0.0f,0.0f };
		float		tranlation[3] = { 0.0f,0.0f,0.0f };
		bool		m_isSeen = true;
	};

private:
	IControllerPtr							m_terrainController;
	MeshGroupTransformation					m_GroupTrans;
	std::vector<MeshTransformation>			m_MeshElementsTrans;
	TrajectoryTransformation				m_TrajectoryTrans;

	MeshGroupState							m_TerrainsState;
	FlythroughState							m_flythroughState;
	Explore3DState							m_explore3dState;
	GeneralModelState						m_generalState;
	std::wstring							m_outputDir;
	int										m_frame;
	bool									m_show_HelpWindow		= false;
	bool									m_show_GeneralWin		= true;
	bool									m_show_Explore3DWin		= false;
	bool									m_show_FlythroughWin	= false;
	bool									m_isFlythroughModeOn	= false;
	bool									m_isRecordingOn			= false;

public:
	bool Initalize(Microsoft::WRL::ComPtr<ID3D11Device> m_device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext, IControllerPtr controller);

	void BeginFrame();
	void EndFrame();
	void DisplayWindows();

	void ShowHelp();
	void ShowGeneralWindow();
	void ShowExplore3DWindow();
	void ShowFlythroughWindow();

	void Shutdown();

	void HandleIModelState(const MeshGroupState&) override;
	void HandleIModelState(const FlythroughState&) override;
	void HandleIModelState(const Explore3DState&) override;
	void HandleIModelState(const GeneralModelState&) override;

	void SetOutputDirectory(const std::wstring& dir);
	void SetIsRecordingOn(bool);

private:
	void Help();
	void GeneralWindow();
	void FlythroughWindow();
	void Explore3DWindow();
	void TerrainListBox();
	void TerrainPopUp(unsigned int terrainId, MeshTransformation& t);
	void TrajectoryPopUp( TrajectoryTransformation& t);

	std::vector<std::string> CollectTerrainIDNames(void);
};

#endif