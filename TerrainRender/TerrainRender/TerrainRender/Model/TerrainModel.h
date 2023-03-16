#ifndef TERRAIN_MODEL_H
#define TERRAIN_MODEL_H

#include "IModel.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "Camera.h"
#include "Position.h"
#include "SunPosition.h"
#include "IRenderable.h"
#include "Light.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "Event/CameraRotateEvent.h"
#include "Event/CameraMoveEvent.h"
#include "Event/FileSelectEvent.h"
#include "Persistence/DataAccess.h"
#include "CompositeRenderable.h"
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d3d11.lib")

class TerrainModel : public IModel
{
private:
	VertexShader				m_vertexShader;
	PixelShader					m_pixelShader;
	Camera						m_camera;
	Position					m_position;
	CompositeRenderable			m_scene;
	Light						m_light;
	SunPosition					m_sunPosition;
	ID3D11Device*				m_device;
	ID3D11DeviceContext*		m_deviceContext;
	IDataAccess*				m_persistence;

public:

	TerrainModel();
	TerrainModel(const TerrainModel&) = delete;
	~TerrainModel();
	bool	Initalize(HWND hwnd, IDataAccess* persistence, ID3D11Device* device, int screenWidth, int screenHeight, float screenNear, float screenDepth, float fieldOfView = (DirectX::XM_PI / 4.0f));
	void	Shutdown();
	bool	Render(ID3D11DeviceContext* deviceContext);
	void	OnMoveCamera(const CameraMoveEvent::Event& event);
	void	OnRotateCamera(const CameraRotateEvent::Event& event);
	void	OnFileSelect(const FileSelectEvent::Event& event);

	bool	LoadTerrain(const wchar_t* filepath);
	bool	LoadCameraTrajectory(const wchar_t* filepath);



};

#endif