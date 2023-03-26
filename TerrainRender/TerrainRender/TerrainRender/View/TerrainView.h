#ifndef TERRAIN_VIEW_H
#define TERRAIN_VIEW_H
#include <DirectXMath.h>
#include "IView.h"
#include <vector>
#include <memory>
#include "D3DView.h"
#include "GuiView.h"
#include "../Controller/IController.h"
#include "../Model/TerrainModel.h"
//TODO : TerrainModel to IMODEL


class IModel;


class TerrainView : public IView
{
private:
	D3DView					m_d3dView;
	GuiView					m_guiView;

	TerrainModelPtr			m_terrainModel;
	IControllerPtr			m_terrainController;

	bool			Render();
public:

	TerrainView();
	TerrainView(const TerrainView&) = delete;
	~TerrainView();
	bool Initalize(HWND hwnd, float screenWidth, float screenHeight, bool fullscreen = false, bool vsync = true);
	void Shutdown();
	bool RenderFrame();
	//void Update(const ModelEvent::Event& event) override;
	//void ExecuteControl(eventType eventType, const void* data);

	void SetController(IControllerPtr terrainController);
	void SetModel(TerrainModelPtr terrainModel);

	void HandleIRenderableInfo(const std::vector<IRenderableInformation>&) override;

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();


};
typedef std::shared_ptr<TerrainView> TerrainViewPtr;

#endif