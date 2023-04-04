#include "TerrainModel.h"
#include "../resource.h"
#include "PolyLine.h"

TerrainModel::TerrainModel() = default;
TerrainModel::~TerrainModel() = default;


bool TerrainModel::Initalize(HWND hwnd, IDataAccess* persistence, ID3D11Device* device, int screenWidth, int screenHeight, float screenNear, float screenDepth, float fieldOfView)
{
	bool bresult;

	if (persistence == nullptr || device == nullptr)
		return false;

	this->m_persistence = persistence;

	this->m_device = device;

	bresult = this->m_vertexShaderMesh.Initialize(device, hwnd);
	if (!bresult)
	{
		return false;
	}

	bresult = this->m_vertexShaderPolyLine.Initialize(device, hwnd);
	if (!bresult)
	{
		return false;
	}

	bresult = this->m_pixelShaderMesh.Initialize(device, hwnd);
	if (!bresult)
	{
		return false;
	}

	bresult = this->m_pixelShaderPolyLine.Initialize(device, hwnd);
	if (!bresult)
	{
		return false;
	}

	this->m_camera.Initialize(screenWidth, screenHeight, screenNear, screenDepth, fieldOfView);
	this->m_position.Initialize(&this->m_camera);

	this->m_meshes.Initialize(device, &m_vertexShaderMesh, &m_pixelShaderMesh, NULL, NULL);
	this->m_polylines.Initialize(device, &m_vertexShaderPolyLine, &m_pixelShaderPolyLine, NULL, NULL);
	this->AddGrid(2000, { 1.0f, 1.0f, 1.0f, 1.0f }, 200, 200);

	return true;
}

void TerrainModel::Shutdown()
{
	m_vertexShaderMesh.Shutdown();
	m_pixelShaderMesh.Shutdown();
	m_pixelShaderPolyLine.Shutdown();
	m_vertexShaderPolyLine.Shutdown();
	m_meshes.Shutdown();
	m_polylines.Shutdown();
}

bool TerrainModel::Render(ID3D11DeviceContext* deviceContext)
{
	this->m_camera.Render();

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();

	this->m_meshes.Render(deviceContext,worldMatrix, m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix(), m_light);

	this->m_polylines.Render(deviceContext, worldMatrix, m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix(), m_light);

	return true;
}

void TerrainModel::Flythrough(unsigned message, float* elapsedMillisec, unsigned* frameNum)
{
	switch (message)
	{
		//TODO what happen when the trajectory ends
	case IDM_CAMERA_TRAJECTORY_START:
	case IDM_CAMERA_TRAJECTORY_STOP:
	{
		this->m_cameraTrajectory.Reset();
		this->m_light.UpdateSunPosition(m_cameraTrajectory.GetCurrentEpochTime().getSeconds(), m_lat, m_longitude);
		break;
	}
	case IDM_CAMERA_TRAJECTORY_NEXT_FRAME:
	{
		this->m_cameraTrajectory.UpdateCamera(*elapsedMillisec);
		this->m_light.UpdateSunPosition(m_cameraTrajectory.GetCurrentEpochTime().getSeconds(), m_lat, m_longitude);
		break;
	}
	case IDM_CAMERA_TRAJECTORY_FRAME:
	{
		this->m_cameraTrajectory.SetCurrentFrame(*frameNum);
		this->m_cameraTrajectory.UpdateCamera(NULL);
		this->m_light.UpdateSunPosition(m_cameraTrajectory.GetCurrentEpochTime().getSeconds(), m_lat, m_longitude);
		break;
	}
	}
	FlythroughState state = CollectFlythroughState();
	m_modelMessageSystem.PublishModelState(state);
}
void TerrainModel::MoveCamera(unsigned message, float timeElapsed)
{
	switch (message)
	{
	case IDM_CAMERA_MOVE_FORWARD:
	{
		this->m_position.MoveForward(timeElapsed);
		break;
	}
	case IDM_CAMERA_MOVE_BACK:
	{
		this->m_position.MoveBack(timeElapsed);
		break;
	}
	case IDM_CAMERA_MOVE_LEFT:
	{
		this->m_position.MoveLeft(timeElapsed);
		break;
	}
	case IDM_CAMERA_MOVE_RIGHT:
	{
		this->m_position.MoveRight(timeElapsed);
		break;
	}
	case IDM_CAMERA_MOVE_UP:
	{
		this->m_position.MoveUp(timeElapsed);
		break;
	}
	case IDM_CAMERA_MOVE_DOWN:
	{
		this->m_position.MoveDown(timeElapsed);
		break;
	}
	}
	CollectExplore3DState();
	m_modelMessageSystem.PublishModelState(m_explore3DState);
}

void TerrainModel::RotateCamera(unsigned message, float pitch, float yaw)
{
	switch (message)
	{
	case IDM_CAMERA_ROTATE:

		this->m_position.RotatePitchYaw(pitch, yaw);
		break;
	}
}

bool TerrainModel::LoadTerrain(const wchar_t* filepath)
{
	VertexMesh*				pVertices;
	UINT					vertexCount;
	std::vector<VertexMesh> vertices;


	bool bresult = m_persistence->LoadTerrain(filepath);
	if (bresult)
	{
		const std::vector<Facet>& facets = m_persistence->GetFacets();
		for (const Facet& facet : facets)
		{
			for (int i = 0; i < 3; i++)
			{
			    VertexMesh vertex;
			    vertex.normal = { (float)facet.normal[0], (float)facet.normal[2],(float)facet.normal[1] };
			    vertex.position = { (float)facet.position[2-i][0], (float)facet.position[2-i][2], (float)facet.position[2-i][1] };
				vertex.color = { 1.0f, 0.5f, 0.5f, 1.0f };
			
			    vertices.push_back(vertex);
			}
		}

		pVertices = &vertices.at(0);
		vertexCount = vertices.size();
		PolygonMeshCreator creator;
		this->m_meshes.Add(pVertices, vertexCount, creator, filepath);
		this->m_modelMessageSystem.PublishModelState(CollectIRenderableInfo());
	}
	return bresult;

}

bool	TerrainModel::LoadTerrainProject(const std::vector<std::wstring>& files)
{
	for (const std::wstring& filepath : files)
	{
		this->LoadTerrain(filepath.c_str());
	}
	return true;

}

bool	TerrainModel::LoadCameraTrajectory(const wchar_t* filepath)
{
	std::vector<CameraPose> cameraPoses;
	bool result = m_persistence->LoadCameraTrajectory(filepath, cameraPoses);
	this->m_cameraTrajectoryIsloaded = result;
	if (result)
	{
		std::vector<VertexPolyLine> vertices;
		for (const CameraPose& camerapose : cameraPoses)
		{
			VertexPolyLine vertex;
			vertex.position = { (float)camerapose.east,-(float)camerapose.down,(float)camerapose.north };
			vertex.color = { 1.0f, 0.0f, 1.0f, 1.0f };
			vertices.push_back(vertex);
		}
		PolyLineCreator creator;
		m_polylines.Add(&vertices.at(0), vertices.size(), creator, filepath);
		IRendarablePtr<VertexPolyLine> polyline = m_polylines.GetLastAddedComponent();
		m_cameraTrajectory.Initialize(cameraPoses, polyline, &m_camera);

		this->m_modelMessageSystem.PublishModelState(CollectIRenderableInfo());
		this->m_modelMessageSystem.PublishModelState(CollectFlythroughState());
	}

	return result;
}

bool TerrainModel::IsTrajectoryLoaded(void) const
{
	return m_cameraTrajectoryIsloaded;
}
void TerrainModel::ResetCamera()
{
	this->m_camera.SetLookAtPos({ 0,0,0 });
}

void TerrainModel::TransformIRenderable(unsigned message, unsigned id,  float parameters[])
{
	switch (message)
	{
	case IDM_TRANSFORMATION_IRENDERABLE_ROTATION:
	{
		m_polylines.RotateComponent(id, parameters[0], parameters[1], parameters[2]);
		m_meshes.RotateComponent(id, parameters[0], parameters[1], parameters[2]);
		break;
	}
	case IDM_TRANSFORMATION_IRENDERABLE_SCALE:
	{
		m_polylines.ScaleComponent(id, parameters[0], parameters[0], parameters[0]);
		m_meshes.ScaleComponent(id, parameters[0], parameters[0], parameters[0]);
		break;
	}
	case IDM_TRANSFORMATION_IRENDERABLE_TRANSLATION:
	{
		m_polylines.TranslateComponent(id, parameters[0], parameters[1], parameters[2]);
		m_meshes.TranslateComponent(id, parameters[0], parameters[1], parameters[2]);
		break;
	}
	case IDM_TRANSFORMATION_IRENDERABLE_COLOR:
	{
		m_meshes.SetColorComponent(id, parameters[0], parameters[1], parameters[2], parameters[3]);
	}
	default:
		break;
	}

}

void TerrainModel::UpdateCameraProperties(unsigned message, float data)
{
	switch (message)
	{
	case IDM_SET_CAMERA_SPEED:
	{
		this->m_position.SetSpeed(data);
		break;
	}
	case IDM_SET_CAMERA_ROTATION_SPEED:
	{
		this->m_position.SetRotationSpeed(data);
		break;
	}

	case IDM_SET_CAMERA_FIELD_OF_VIEW:
	{
		this->m_camera.SetFieldOfView(data);
		break;
	}
	case IDM_SET_CAMERA_ASPECT_RATIO:
	{
		//this->m_position.SetRotationSpeed(data);
		break;
	}
	case IDM_SET_CAMERA_ASPECT_NEAR_SCREEN:
	{
		this->m_camera.SetNearScreen(data);
		break;
		//this->m_position.SetRotationSpeed(data);
	}
	case IDM_SET_CAMERA_ASPECT_FAR_SCREEN:
	{
		this->m_camera.SetFarScreen(data);
		break;
		//this->m_position.SetRotationSpeed(data);
	}
	default:
		break;
	}
}

FlythroughState	TerrainModel::CollectFlythroughState(void)
{
	FlythroughState state;
	//TODO currentFrame
	state.currentFrame = m_cameraTrajectory.GetCurrentFrameNum();
	state.numberOfFrame = m_cameraTrajectory.GetNumberOfFrame();
	state.IsTrajectoryLoaded = this->IsTrajectoryLoaded();
	state.currentEpochTime = m_cameraTrajectory.GetCurrentEpochTime();
	state.currentPosition = m_camera.GetPositionFloat3();
	state.currentRotation = m_camera.GetRotationRadFloat3();
	state.currentSunPosition.azimuth = m_light.GetAzimuth();
	state.currentSunPosition.elevation = m_light.GetElevation();
	
	return state;
}

void TerrainModel::CollectExplore3DState(void)
{
	//TODO SET DATE
	m_explore3DState.currentEpochTime = EpochTime(1664534690, 0);
	m_explore3DState.currentPosition	= m_camera.GetPositionFloat3();
	m_explore3DState.currentRotation = m_camera.GetRotationRadFloat3();
	m_explore3DState.currentSunPosition.azimuth = m_light.GetAzimuth();
	m_explore3DState.currentSunPosition.elevation = m_light.GetElevation();
}
std::vector<IRenderableState> TerrainModel::CollectIRenderableInfo()
{
	std::vector<IRenderableState> meshInfo;
	m_meshes.CollectIRenderableState(meshInfo);
	return meshInfo;
}

void TerrainModel::ClearTerrain(void)
{
	this->m_meshes.ClearRenderables();
	this->m_modelMessageSystem.PublishModelState(CollectIRenderableInfo());

}

void TerrainModel::ClearCameraTrajectory(void)
{

}

void TerrainModel::AddGrid(float size, DirectX::XMFLOAT4 color, int gridX, int gridZ)
{
	std::vector<VertexPolyLine> vertices;
	if (size <= 0 || gridX <= 0 || gridZ <= 0)
		return;

	float gridXStep = size / gridX;
	float gridZStep = size / gridZ;

	for (int i = 0; i <= gridX; i++)
	{
		VertexPolyLine polygon;
		polygon.color = color;
		float zCoord = size / 2;
		float xCoord = (-size / 2) + i * gridXStep;
		polygon.position = { xCoord, 0.0f, zCoord };
		vertices.push_back(polygon);

		polygon.position = { xCoord, 0.0f, -zCoord };
		vertices.push_back(polygon);
	}

	for (int i = 0; i <= gridZ; i++)
	{
		VertexPolyLine polygon;
		polygon.color = color;
		float xCoord = size / 2;
		float zCoord = (-size / 2) + i * gridZStep;
		polygon.position = { xCoord, 0.0f, zCoord };
		vertices.push_back(polygon);

		polygon.position = { -xCoord, 0.0f, zCoord };
		vertices.push_back(polygon);
	}

	VertexPolyLine* pVertex = &vertices[0];
	unsigned verteCount = vertices.size();
	LineListCreator	lineListCreator;
	this->m_polylines.Add(pVertex, verteCount, lineListCreator, L"Grid");
	this->m_modelMessageSystem.PublishModelState(CollectIRenderableInfo());
}


