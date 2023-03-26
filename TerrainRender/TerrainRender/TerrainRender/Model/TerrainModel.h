#ifndef TERRAIN_MODEL_H
#define TERRAIN_MODEL_H

#include "IModel.h"
#include "PixelShaderMesh.h"
#include "VertexShaderMesh.h"
#include "VertexShaderPolyLine.h"
#include "PixelShaderPolyLine.h"
#include "DepthPixelShader.h"
#include "DepthVertexShader.h"
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
#include "CameraTrajectory.h"
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d3d11.lib")

class TerrainModel : public IModel
{
private:
	VertexShaderMesh							m_vertexShaderMesh;
	PixelShaderMesh								m_pixelShaderMesh;
	VertexShaderPolyLine						m_vertexShaderPolyLine;
	PixelShaderPolyLine							m_pixelShaderPolyLine;
	DepthPixelShader							m_depthPixelShader;
	DepthVertexShader							m_depthVertexShader;

	Position									m_position;
	CompositeRenderable<VertexMesh>				m_meshes;
	CompositeRenderable<VertexPolyLine>			m_polylines;
	Light										m_light;
	SunPosition									m_sunPosition;
	CameraTrajectory							m_cameraTrajectory;

	ID3D11Device*								m_device;
	ID3D11DeviceContext*						m_deviceContext;
	IDataAccess*								m_persistence;
	bool										m_cameraTrajectoryIsloaded = false;


public:
	Camera									m_camera;
	


public:

	TerrainModel();
	TerrainModel(const TerrainModel&) = delete;
	~TerrainModel();
	bool	Initalize(HWND hwnd, IDataAccess* persistence, ID3D11Device* device, int screenWidth, int screenHeight, float screenNear, float screenDepth, float fieldOfView = (DirectX::XM_PI / 4.0f));
	void	Shutdown();
	bool	Render(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* depthMapTexture);
	bool	RenderShadowMap(ID3D11DeviceContext* deviceContext);

	bool	LoadTerrain(const wchar_t* filepath);
	bool	LoadCameraTrajectory(const wchar_t* filepath);
	bool	LoadTerrainProject(const std::vector<std::wstring>& files);

	bool	IsTrajectoryLoaded(void) const;

	void	Flythrough(unsigned message, double elapsedMillisec);
	void	MoveCamera(unsigned message, float timeElapsed);
	void	RotateCamera(unsigned message, float pitch, float yaw);
	void	ResetCamera(void);
	void	UpdateCameraProperties(unsigned message, float data);
private:
	void	AddGrid(float size, DirectX::XMFLOAT4 color, int gridX, int gridZ);
};

#endif