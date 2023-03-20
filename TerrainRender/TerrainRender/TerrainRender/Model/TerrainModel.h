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
#include "Persistence/DataAccess.h"
#include "CompositeRenderable.h"
#include "RenderableCreator.h"
#include "VertexShaderPolygon.h"
#include "CameraTrajectory.h"
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d3d11.lib")

class TerrainModel : public IModel
{
private:
	VertexShader							m_vertexShader;
	VertexShaderPolygon						m_vertexShaderPolygon;
	PixelShader								m_pixelShader;
	Position								m_position;
	CompositeRenderable<Vertex>				m_meshes;
	CompositeRenderable<VertexPolygon>		m_polygons;
	Light									m_light;
	SunPosition								m_sunPosition;
	CameraTrajectory						m_cameraTrajectory;

	ID3D11Device*							m_device;
	ID3D11DeviceContext*					m_deviceContext;
	IDataAccess*							m_persistence;
	PolygonCreator							m_polygonCreator;
	PolygonMeshCreator						m_polygonMeshCreator;

public:
	Camera									m_camera;
	


public:

	TerrainModel();
	TerrainModel(const TerrainModel&) = delete;
	~TerrainModel();
	bool	Initalize(HWND hwnd, IDataAccess* persistence, ID3D11Device* device, int screenWidth, int screenHeight, float screenNear, float screenDepth, float fieldOfView = (DirectX::XM_PI / 4.0f));
	void	Shutdown();
	bool	Render(ID3D11DeviceContext* deviceContext);




	bool	LoadTerrain(const wchar_t* filepath);
	bool	LoadCameraTrajectory(const wchar_t* filepath);

	void	Flythrough(unsigned message, double elapsedMillisec);
	void	MoveCamera(unsigned message, float timeElapsed);
	void	RotateCamera(unsigned message, float pitch, float yaw);
	void	ResetCamera(void);
	void	UpdateCameraProperties(unsigned message, float data);
private:
	void	AddGrid(float size, DirectX::XMFLOAT4 color, int gridX, int gridZ);
};

#endif