#include "TerrainModel.h"
#include "../resource.h"
#include "PolyLine.h"
#include "../StringConverter.h"

TerrainModel::TerrainModel() = default;
TerrainModel::~TerrainModel() = default;


bool TerrainModel::Initalize(HWND hwnd, IDataAccessPtr persistence, Microsoft::WRL::ComPtr<ID3D11Device> device, int screenWidth, int screenHeight, float screenNear, float screenDepth, float fieldOfView)
{
	bool bresult;

	if (persistence == nullptr || device == nullptr)
		return false;

	this->m_persistence = persistence;

	this->m_device = device;

	m_vertexShaderMesh = std::make_shared<VertexShaderMesh>();
	m_pixelShaderMesh = std::make_shared<PixelShaderMesh>();
	m_vertexShaderPolyLine = std::make_shared<VertexShaderPolyLine>();
	m_pixelShaderPolyLine = std::make_shared<PixelShaderPolyLine>();

	bresult = this->m_vertexShaderMesh->Initialize(device, hwnd);
	if (!bresult)
	{
		return false;
	}

	bresult = this->m_vertexShaderPolyLine->Initialize(device, hwnd);
	if (!bresult)
	{
		return false;
	}

	bresult = this->m_pixelShaderMesh->Initialize(device, hwnd);
	if (!bresult)
	{
		return false;
	}

	bresult = this->m_pixelShaderPolyLine->Initialize(device, hwnd);
	if (!bresult)
	{
		return false;
	}

	this->m_camera.Initialize(screenWidth, screenHeight, screenNear, screenDepth, fieldOfView);
	this->m_cameraPositioner.Initialize(&this->m_camera);

	this->m_meshes.Initialize(device, m_vertexShaderMesh, m_pixelShaderMesh, NULL, NULL, NULL, NULL);
	this->m_polylines.Initialize(device, m_vertexShaderPolyLine, m_pixelShaderPolyLine, NULL, NULL, NULL, NULL);
	this->m_light.UpdateSunPosition(m_cameraPositioner.GetCurrentEpochTime().getSeconds(), m_llacoordinate.latitude, m_llacoordinate.longitude);
	PublishModelState();
	this->AddGrid(2000, { 1.0f, 1.0f, 1.0f, 1.0f }, 200, 200);

	return true;
}

void TerrainModel::Resize(unsigned screenWidth, unsigned screenHeight)
{
	this->m_camera.Resize(screenWidth, screenHeight);
}

void TerrainModel::Shutdown()
{
	m_vertexShaderMesh->Shutdown();
	m_pixelShaderMesh->Shutdown();
	m_pixelShaderPolyLine->Shutdown();
	m_vertexShaderPolyLine->Shutdown();
	m_meshes.Shutdown();
	m_polylines.Shutdown();
}

bool TerrainModel::Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext)
{
	this->m_camera.Render();

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();

	this->m_meshes.Render(deviceContext,worldMatrix, m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix(), m_light);

	this->m_polylines.Render(deviceContext, worldMatrix, m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix(), m_light);
	return true;
}



void TerrainModel::HandleFlythroughMode(unsigned message, float* elapsedMillisec, unsigned* frameNum)
{
	switch (message)
	{
		//TODO what happen when the trajectory ends
	case IDM_CAMERA_TRAJECTORY_START:
	case IDM_CAMERA_TRAJECTORY_STOP:
	{
		this->m_cameraTrajectory.Reset();
		break;
	}
	case IDM_CAMERA_TRAJECTORY_NEXT_FRAME:
	{
		this->m_cameraTrajectory.UpdateCamera(*elapsedMillisec);
		break;
	}
	case IDM_CAMERA_TRAJECTORY_SET_FRAME:
	{
		this->m_cameraTrajectory.SetCurrentFrame(*frameNum);
		this->m_cameraTrajectory.UpdateCamera(NULL);
		break;
	}
	}
	this->m_light.UpdateSunPosition(m_cameraTrajectory.GetCurrentEpochTime().getSeconds(), m_llacoordinate.latitude, m_llacoordinate.longitude);
	PublishModelState();
}

void	TerrainModel::HandleExplore3DMode(unsigned message, float* fparams)
{
	switch (message)
	{
	case IDM_CAMERA_MOVE_FORWARD:
	case IDM_CAMERA_MOVE_BACK:
	case IDM_CAMERA_MOVE_LEFT:
	case IDM_CAMERA_MOVE_RIGHT:
	case IDM_CAMERA_MOVE_UP:
	case IDM_CAMERA_MOVE_DOWN:
	{
		this->MoveCamera(message, *fparams);
		break;
	}
	case IDM_CAMERA_ROTATE:
	{
		this->RotateCamera(message, fparams[0], fparams[1]);
		break;
	}
	default:
		break;
	}
	this->m_light.UpdateSunPosition(m_cameraPositioner.GetCurrentEpochTime().getSeconds(), m_llacoordinate.latitude, m_llacoordinate.longitude);
	PublishModelState();
}

void TerrainModel::MoveCamera(unsigned message, float timeElapsed)
{
	switch (message)
	{
	case IDM_CAMERA_MOVE_FORWARD:
	{
		this->m_cameraPositioner.MoveForward(timeElapsed);
		break;
	}
	case IDM_CAMERA_MOVE_BACK:
	{
		this->m_cameraPositioner.MoveBack(timeElapsed);
		break;
	}
	case IDM_CAMERA_MOVE_LEFT:
	{
		this->m_cameraPositioner.MoveLeft(timeElapsed);
		break;
	}
	case IDM_CAMERA_MOVE_RIGHT:
	{
		this->m_cameraPositioner.MoveRight(timeElapsed);
		break;
	}
	case IDM_CAMERA_MOVE_UP:
	{
		this->m_cameraPositioner.MoveUp(timeElapsed);
		break;
	}
	case IDM_CAMERA_MOVE_DOWN:
	{
		this->m_cameraPositioner.MoveDown(timeElapsed);
		break;
	}
	}
}

void TerrainModel::RotateCamera(unsigned message, float pitch, float yaw)
{
	switch (message)
	{
	case IDM_CAMERA_ROTATE:

		this->m_cameraPositioner.RotatePitchYaw(pitch, yaw);
		break;
	}
	PublishModelState();
}

bool TerrainModel::LoadTerrainSoftEdges(const wchar_t* filepath)
{
	VertexMesh* pVertices;
	UINT									vertexCount;
	UINT									indexCount;
	std::vector<VertexMesh>					verticesMesh;
	std::vector<unsigned long>				indices;
	unsigned long* pIndices;


	bool bresult = m_persistence->LoadTerrainSoftEdges(filepath);
	if (bresult)
	{
		const std::vector<StlVertex>& vertices = m_persistence->GetVertices_Soft();
		const std::vector<CornerIndices>& facetIndices = m_persistence->GetIndices_Soft();
		for (const CornerIndices& facet : facetIndices)
		{
			for (int i = 0; i < 3; i++)
			{
				const size_t& index = facet.corner[2-i];
				indices.push_back(index);
			}
		}
		for (const StlVertex& v : vertices)
		{
				VertexMesh vertexMesh;
				vertexMesh.normal = { (float)v.normal.x, (float)v.normal.z,(float)v.normal.y };
				vertexMesh.position = { (float)v.pos.x, (float)v.pos.z, (float)v.pos.y };
				vertexMesh.color = { 1.0f, 0.5f, 0.5f, 1.0f };
				verticesMesh.push_back(vertexMesh);
		}

		pVertices = &verticesMesh.at(0);
		vertexCount = verticesMesh.size();
		pIndices = &indices.at(0);
		indexCount = indices.size();

		PolygonMeshCreator creator;
		this->m_meshes.Add(pVertices, pIndices, vertexCount, indexCount, creator, filepath);

		PublishModelState();
	}
	return bresult;
}

bool TerrainModel::LoadTerrainSharpEdges(const wchar_t* filepath)
{
	VertexMesh*								pVertices;
	UINT									vertexCount;
	UINT									indexCount;
	std::vector<VertexMesh>					vertices;
	std::vector<unsigned long>				indices;
	unsigned long*							pIndices;


	bool bresult = m_persistence->LoadTerrainSharpEdges(filepath);
	if (bresult)
	{
		const std::vector<stlFacet>& facets = m_persistence->GetFacets();
		unsigned index = 0;
		for (const stlFacet& facet : facets)
		{
			for (int i = 0; i < 3; i++)
			{
			    VertexMesh vertex;
			    vertex.normal = { (float)facet.normal[0], (float)facet.normal[2],(float)facet.normal[1] };
			    vertex.position = { (float)facet.position[2-i][0], (float)facet.position[2-i][2], (float)facet.position[2-i][1] };
				vertex.color = { 1.0f, 0.5f, 0.5f, 1.0f };
			
			    vertices.push_back(vertex);
				indices.push_back(index);
				index++;
			}
		}

		pVertices = &vertices.at(0);
		vertexCount = vertices.size();
		pIndices = &indices.at(0);
		indexCount = indices.size();

		PolygonMeshCreator creator;
		this->m_meshes.Add(pVertices, pIndices, vertexCount, indexCount, creator, filepath);

		PublishModelState();
	}
	return bresult;
}

bool	TerrainModel::LoadTerrainSharpEdges_Project(const std::vector<std::wstring>& files)
{
	for (const std::wstring& filepath : files)
	{
		this->LoadTerrainSharpEdges(filepath.c_str());
	}
	return true;

}

bool	TerrainModel::LoadTerrainSoftEdges_Project(const std::vector<std::wstring>& files)
{
	for (const std::wstring& filepath : files)
	{
		this->LoadTerrainSoftEdges(filepath.c_str());
	}
	return true;

}

bool	TerrainModel::LoadParameters(const wchar_t* filepath)
{
	ParameterFile params;
	bool bresult = m_persistence->LoadParameterFile(filepath, params);
	if (bresult)
	{
		//Set world origo
		this->m_llacoordinate = params.origo;

		//Set Trajectory
		this->m_cameraTrajectory.Move(params.trajectory.translation);
		this->m_cameraTrajectory.Rotate(params.trajectory.rotation);

		//Set Terrain
		m_meshes.Rotate(params.terrain.rotation.x, params.terrain.rotation.y, params.terrain.rotation.z);
		m_meshes.Translate(params.terrain.translation.x, params.terrain.translation.y, params.terrain.translation.z);

		for (auto it = params.terrain.colors.begin(); it != params.terrain.colors.end(); it++)
		{
			float r, g, b, a;
			std::wstring componentName = StringConverter::StringToWide(it->first);
			r = it->second.x;
			g = it->second.y;
			b = it->second.z;
			a = it->second.w;
			m_meshes.SetColorComponent(componentName, r, g, b, a);
		}
		PublishModelState();
	}
	return false;
}

bool	TerrainModel::LoadCameraTrajectory(const wchar_t* filepath)
{
	std::vector<CameraPose> cameraPoses;
	bool result = m_persistence->LoadCameraTrajectory(filepath, cameraPoses);
	if (result)
	{
		ClearCameraTrajectory();
		std::vector<VertexPolyLine> vertices;
		for (const CameraPose& camerapose : cameraPoses)
		{
			VertexPolyLine vertex;
			vertex.position = { (float)camerapose.east,-(float)camerapose.down,(float)camerapose.north };
			vertex.color = { 1.0f, 0.0f, 1.0f, 1.0f };
			vertices.push_back(vertex);
		}
		PolyLineCreator creator;
		
		m_polylines.Add(&vertices.at(0), NULL, vertices.size(), NULL, creator, filepath);
		IRendarablePtr<VertexPolyLine> polyline = m_polylines.GetLastAddedComponent();
		m_cameraTrajectory.Initialize(cameraPoses, polyline, &m_camera);
		PublishModelState();
	}

	return result;
}

bool TerrainModel::IsTrajectoryInitialized(void) const
{
	return m_cameraTrajectory.IsInitialized();
}
void TerrainModel::ResetCamera()
{
	this->m_camera.SetLookAtPos({ 0,0,0 });
	PublishModelState();
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
		break;
	}

	case IDM_IRENDERABLE_ISSEEN:
	{
		bool m_isSeen = parameters[0] > 0 ? 1 : 0;
		m_meshes.SetIsSeenComponent(id, m_isSeen);
		m_polylines.SetIsSeenComponent(id, m_isSeen);
		break;
	}
	default:
		break;
	}
	PublishModelState();

}

void TerrainModel::UpdateCameraProperties(unsigned message, float data)
{
	switch (message)
	{
	case IDM_SET_CAMERA_SPEED:
	{
		this->m_cameraPositioner.SetSpeed(data);
		break;
	}
	case IDM_SET_CAMERA_ROTATION_SPEED:
	{
		this->m_cameraPositioner.SetRotationSpeed(data);
		break;
	}

	case IDM_SET_CAMERA_FIELD_OF_VIEW:
	{
		this->m_camera.SetFieldOfView(data);
		break;
	}
	case IDM_SET_CAMERA_ASPECT_NEAR_SCREEN:
	{
		this->m_camera.SetNearScreen(data);
		break;
	}
	case IDM_SET_CAMERA_ASPECT_FAR_SCREEN:
	{
		this->m_camera.SetFarScreen(data);
		break;
	}
	default:
		break;
	}
	PublishModelState();
}

FlythroughState	TerrainModel::CollectFlythroughState(void) const
{
	FlythroughState state;
	if (m_cameraTrajectory.IsInitialized())
	{
		state.currentFrame					= m_cameraTrajectory.GetCurrentFrameNum();
		state.numberOfFrame					= m_cameraTrajectory.GetNumberOfFrame();
		state.IsTrajectoryInitialized		= this->IsTrajectoryInitialized();
		state.currentEpochTime				= m_cameraTrajectory.GetCurrentEpochTime();
		state.currentCameraPosition			= m_camera.GetPositionF3();
		state.currentCameraRotation			= m_camera.GetRotationF3();
		state.currentSunPosition.azimuth	= m_light.GetAzimuth();
		state.currentSunPosition.elevation	= m_light.GetElevation();
		state.trajectoryPolyLine.push_back(m_cameraTrajectory.GetTrajectoryPolyLineState());
		state.origo = this->m_llacoordinate;
	}
	return state;
}

CameraState		TerrainModel::CollectCameraState(void) const
{
	return CameraState{ m_camera.GetFOVrad(), m_camera.GetNearScreen(), m_camera.GetFarScreen() };
}
Explore3DState TerrainModel::CollectExplore3DState(void) const
{
	Explore3DState state;
	state.speed							= m_cameraPositioner.GetSpeed();
	state.rotationSpeed					= m_cameraPositioner.GetRotationSpeed();
	state.currentEpochTime				= m_cameraPositioner.GetCurrentEpochTime();
	state.currentCameraPosition			= m_camera.GetPositionF3();
	state.currentCameraRotation			= m_camera.GetRotationF3();
	state.currentSunPosition.azimuth	= m_light.GetAzimuth();
	state.currentSunPosition.elevation	= m_light.GetElevation();
	state.origo = this->m_llacoordinate;

	return state;
}
std::vector<IRenderableState> TerrainModel::CollectTerrainMeshState() const
{
	std::vector<IRenderableState> meshInfo;
	m_meshes.CollectIRenderableState(meshInfo);
	return meshInfo;
}

void TerrainModel::ClearTerrain(void)
{
	this->m_meshes.ClearRenderables();
	PublishModelState();
}

void TerrainModel::ClearCameraTrajectory(void)
{
	if (this->m_cameraTrajectory.GetPolyLine() != nullptr)
	{
		m_polylines.Remove(this->m_cameraTrajectory.GetPolyLine());
		this->m_cameraTrajectory.GetPolyLine()->Shutdown();
	}
	this->m_cameraTrajectory.Clear();
	PublishModelState();
}

void TerrainModel::PublishModelState(void) const
{
	this->m_modelMessageSystem.PublishModelState(CollectTerrainMeshState());
	this->m_modelMessageSystem.PublishModelState(CollectFlythroughState());
	this->m_modelMessageSystem.PublishModelState(CollectExplore3DState());
	this->m_modelMessageSystem.PublishModelState(CollectCameraState());
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
	this->m_polylines.Add(pVertex, NULL, verteCount,NULL, lineListCreator, L"Grid");
}


