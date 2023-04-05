#ifndef TERRAIN_MODEL_H
#define TERRAIN_MODEL_H

#include "IModel.h"
#include "PixelShaderMesh.h"
#include "VertexShaderMesh.h"
#include "VertexShaderPolyLine.h"
#include "PixelShaderPolyLine.h"
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
#include "ModelMessageSystem.h"
#include <memory>
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d3d11.lib")

class TerrainModel : public IModel
{
private:

	Explore3DState								m_explore3DState;
	double										m_lat = 47.497913;
	double										m_longitude = 19.040236;

	VertexShaderMesh							m_vertexShaderMesh;
	PixelShaderMesh								m_pixelShaderMesh;
	VertexShaderPolyLine						m_vertexShaderPolyLine;
	PixelShaderPolyLine							m_pixelShaderPolyLine;

	Position									m_position;
	CompositeRenderable<VertexMesh>				m_meshes;
	CompositeRenderable<VertexPolyLine>			m_polylines;
	Light										m_light;
	CameraTrajectory							m_cameraTrajectory;

	ID3D11Device*								m_device;
	ID3D11DeviceContext*						m_deviceContext;
	IDataAccess*								m_persistence;

public:
	Camera										m_camera;
	ModelMessageSystem							m_modelMessageSystem;
	


public:

	TerrainModel();
	TerrainModel(const TerrainModel&) = delete;
	~TerrainModel();
	bool	Initalize(HWND hwnd, IDataAccess* persistence, ID3D11Device* device, int screenWidth, int screenHeight, float screenNear, float screenDepth, float fieldOfView = (DirectX::XM_PI / 4.0f)) override;
	void	Shutdown() override;
	bool	Render(ID3D11DeviceContext* deviceContext) override;

	bool	LoadTerrain(const wchar_t* filepath) override;
	bool	LoadCameraTrajectory(const wchar_t* filepath) override;
	bool	LoadTerrainProject(const std::vector<std::wstring>& files) override;

	bool	IsTrajectoryInitialized(void) const override;

	void	Flythrough(unsigned message, float* elapsedMillisec, unsigned* frameNum) override;
	void	MoveCamera(unsigned message, float timeElapsed) override;
	void	RotateCamera(unsigned message, float pitch, float yaw) override;
	void	ResetCamera(void) override;
	void	UpdateCameraProperties(unsigned message, float data) override;
	void    TransformIRenderable(unsigned message, unsigned id, float parameters[]) override;
	void	ClearTerrain(void) override;
	void	ClearCameraTrajectory(void) override;
private:
	std::vector <IRenderableState>	CollectTerrainMeshState(void);
	FlythroughState					CollectFlythroughState(void);
	void							CollectExplore3DState(void);
	void							AddGrid(float size, DirectX::XMFLOAT4 color, int gridX, int gridZ);
};
typedef std::shared_ptr<TerrainModel> TerrainModelPtr;
#endif