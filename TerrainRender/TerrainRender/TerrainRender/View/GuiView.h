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
	IControllerPtr			m_terrainController;

	//ModelStates:
	std::vector<IRenderableInformation> m_IRenderableInfo;
public:
	bool Initalize(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, IControllerPtr controller);

	void BeginFrame();
	void EndFrame();
	void ShowSettingWindow();
	void HandleIRenderableInfo(const std::vector<IRenderableInformation>&) override;

	void Shutdown();
};