#ifndef TERRAIN_MODEL_H
#define TERRAIN_MODEL_H

///////////////////////////////////////////////////////////////////////////////
// TerrainModel.h
// ==============
// 
// The TerrainModel class is a derived class of IModel and represents a terrain in the program. 
// 
// The reversal of the flow direction is handled by the derived classes of the IModel interface.
//
// The class implements the HandleMessage function responsible for processing messages.
// 
// It initializes the classes associated with the model layer. 
//
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

#include "IModel.h"
#include "PixelShaderMesh.h"
#include "VertexShaderMesh.h"
#include "VertexShaderPolyLine.h"
#include "PixelShaderPolyLine.h"
#include "Camera.h"
#include "CameraPositioner.h"
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
	enum Mode {Explore3D, Flythrough};
private:
	LLACoordinate								m_origoLLA = { 47.497913f , 19.040236f };

	IVertexShaderPtr							m_vertexShaderMesh;
	PixelShaderMeshPtr							m_pixelShaderMesh;
	IVertexShaderPtr							m_vertexShaderPolyLine;
	IPixelShaderPtr								m_pixelShaderPolyLine;

	CompositeRenderable<VertexMesh>				m_meshes;
	CompositeRenderable<VertexPolyLine>			m_polylines;
	CompositeRenderable<VertexPolyLine>			m_linelist;
	Light										m_light;
	CameraPositioner							m_cameraPositioner;
	CameraTrajectory							m_cameraTrajectory;

	IDataAccessPtr								m_persistence;
	Microsoft::WRL::ComPtr<ID3D11Device>		m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>	m_deviceContext;

	Mode										m_mode = Explore3D;

public:
	CameraPtr									m_camera;
	ModelMessageSystem							m_modelMessageSystem;
	
public:
	TerrainModel();
	TerrainModel(const TerrainModel&) = delete;
	~TerrainModel();
	bool	Initalize(IDataAccessPtr persistence, Microsoft::WRL::ComPtr<ID3D11Device> device, int screenWidth, int screenHeight, float screenNear, float screenDepth, float fieldOfView = (DirectX::XM_PI / 3.0)) override;
	bool	HandleMessage(IModelMessageIDs message, const std::vector<std::wstring>& stringParams, const std::vector<float>& fparams, const std::vector<unsigned>& uparams) override;
	bool	Resize(unsigned screenWidth, unsigned screenHeight) override;
	void	Shutdown() override;
	bool	Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext) override;
	bool	IsTrajectoryInitialized(void) const override;

	bool	LoadTerrain_withSharpEdges(const std::wstring& filepath) ;
	bool	LoadTerrain_withSoftEdges(const std::wstring& filepath) ;
	bool	LoadCameraTrajectory(const std::wstring& filepath) ;
	bool	LoadConfigurationFile(const std::wstring& filepath) ;
	bool	LoadProject_withSharpEdges(const std::vector<std::wstring>& files) ;
	bool	LoadProject_withSoftEdges(const std::vector<std::wstring>& files) ;

	bool	HandleFlythroughMode(IModelMessageIDs message, const std::vector<float>& fparams, const std::vector<unsigned>& uparams) ;
	bool	HandleExplore3DMode(IModelMessageIDs message, const std::vector<float>& fparams) ;

	bool	SetLongitudeLatitude(IModelMessageIDs message, const std::vector<float>& fparams) ;
	bool	SetUnixTime(IModelMessageIDs message, unsigned uparam) ;
	bool	ResetCamera(void) ;
	bool	SetCameraProperties(IModelMessageIDs message, float data) ;
	bool    TransformMeshElement(IModelMessageIDs message, unsigned id, const std::vector<float>& fparams) ;

	bool	TransformMeshGroup(IModelMessageIDs message, const std::vector<float>& fparams);
	bool	TransformTrajectory(IModelMessageIDs message, const std::vector<float>& fparams);
	bool	ClearMeshes(void) ;
	bool	ClearCameraTrajectory(void);
	bool	SetMode(IModelMessageIDs message);
	void	UpdateSunPosition();
private:
	void				MoveCamera(IModelMessageIDs message, float timeElapsed);
	void				RotateCamera(IModelMessageIDs message, float pitch, float yaw);
	MeshGroupState		CollectMeshGroupState(void) const;
	FlythroughState		CollectFlythroughState(void) const;
	Explore3DState		CollectExplore3DState(void) const;
	GeneralModelState	CollectCameraState(void) const;
	void				PublishModelState(void) const;
	void				AddGrid(float size, DirectX::XMFLOAT4 color, int gridX, int gridZ);
};
typedef std::shared_ptr<TerrainModel> TerrainModelPtr;
#endif