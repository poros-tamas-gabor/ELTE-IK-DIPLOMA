#include "TerrainModel.h"
#include "../resource.h"
#include "PolyLine.h"
#include "../StringConverter.h"
#include "../ErrorHandler.h"

TerrainModel::TerrainModel() = default;
TerrainModel::~TerrainModel() = default;


bool TerrainModel::Initalize(IDataAccessPtr persistence, Microsoft::WRL::ComPtr<ID3D11Device> device, int screenWidth, int screenHeight, float screenNear, float screenDepth, float fieldOfView)
{
	try
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
		m_camera = std::make_shared<Camera>();

		bresult = this->m_vertexShaderMesh->Initialize(device);
		if (!bresult)
		{
			return false;
		}

		bresult = this->m_vertexShaderPolyLine->Initialize(device);
		if (!bresult)
		{
			return false;
		}

		bresult = this->m_pixelShaderMesh->Initialize(device);
		if (!bresult)
		{
			return false;
		}

		bresult = this->m_pixelShaderPolyLine->Initialize(device);
		if (!bresult)
		{
			return false;
		}

		this->m_camera->Initialize(screenWidth, screenHeight, screenNear, screenDepth, fieldOfView);
		this->m_cameraPositioner.Initialize(this->m_camera);

		this->m_meshes.Initialize(device, m_vertexShaderMesh, m_pixelShaderMesh, NULL, NULL, NULL, NULL);
		this->m_polylines.Initialize(device, m_vertexShaderPolyLine, m_pixelShaderPolyLine, NULL, NULL, NULL, NULL);
		this->m_linelist.Initialize(device, m_vertexShaderPolyLine, m_pixelShaderPolyLine, NULL, NULL, NULL, NULL);
		this->m_light.UpdateSunPosition(m_cameraPositioner.GetCurrentEpochTime().getSeconds(), m_llacoordinate.latitude, m_llacoordinate.longitude);
		PublishModelState();
		this->AddGrid(2000, { 0.6f, 0.6f, 0.6f, 0.6f }, 200, 200);

		return true;
	}
	catch (const COMException& e)
	{
		ErrorHandler::Log(e);
	}
	catch (const TRException& e)
	{
		ErrorHandler::Log(e);
	}
	catch (const std::exception& e)
	{
		ErrorHandler::Log(e);
	}
	catch (...)
	{
		ErrorHandler::Log("Unknown Exceptio: No details available");
	}
	return false;
}

bool TerrainModel::Resize(unsigned screenWidth, unsigned screenHeight)
{
	try
	{
		if (m_camera != nullptr)
		{
			this->m_camera->Resize(screenWidth, screenHeight);
			return true;
		}
		return false;
	}
	catch (const COMException& e)
	{
		ErrorHandler::Log(e);
	}
	catch (const TRException& e)
	{
		ErrorHandler::Log(e);
	}
	catch (const std::exception& e)
	{
		ErrorHandler::Log(e);
	}
	catch (...)
	{
		ErrorHandler::Log("Unknown Exceptio: No details available");
	}
	return false;
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
	this->m_camera->Render();

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();

	this->m_meshes.Render(deviceContext,worldMatrix, m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix(), m_light);

	this->m_polylines.Render(deviceContext, worldMatrix, m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix(), m_light);

	this->m_linelist.Render(deviceContext, worldMatrix, m_camera->GetViewMatrix(), m_camera->GetProjectionMatrix(), m_light);
	return true;
}

bool TerrainModel::HandleMessage(IModelMessageIDs message, const std::vector<std::wstring>& stringParams, const std::vector<float>& fparams, const std::vector<unsigned>& uparams)
{
	try
	{
		bool success = true;
		switch (message)
		{
		case IDM_LOAD_TERRAIN_SHARP:
			success = this->LoadTerrain_withSharpEdges(stringParams.at(0));
			break;

		case IDM_LOAD_PROJECT_SHARP:
			success = this->LoadProject_withSharpEdges(stringParams);
			break;

		case IDM_LOAD_TERRAIN_SOFT:
			success = this->LoadTerrain_withSoftEdges(stringParams.at(0));
			break;

		case IDM_LOAD_PROJECT_SOFT:
			success = this->LoadProject_withSoftEdges(stringParams);
			break;

		case IDM_LOAD_CAMERA_TRAJECTORY:
			success = this->LoadCameraTrajectory(stringParams.at(0));
			break;

		case IDM_LOAD_CONFIGURATION:
			success = this->LoadConfigurationFile(stringParams.at(0));
			break;

		case IDM_E3D_MOVE_FORWARD:
		case IDM_E3D_MOVE_BACK:
		case IDM_E3D_MOVE_LEFT:
		case IDM_E3D_MOVE_RIGHT:
		case IDM_E3D_MOVE_UP:
		case IDM_E3D_MOVE_DOWN:
		case IDM_E3D_ROTATE:
		case IDM_E3D_SET_SPEED:
		case IDM_E3D_SET_ROTATION_SPEED:
			success = HandleExplore3DMode(message, fparams);
			break;

		case IDM_E3D_CAMERA_RESET:
			success = ResetCamera();
			break;

		case IDM_SET_CAMERA_FIELD_OF_VIEW:
		case IDM_SET_CAMERA_NEAR_SCREEN:
		case IDM_SET_CAMERA_FAR_SCREEN:
			success = SetCameraProperties(message, fparams.at(0));
			break;

		case IDM_FLYTHROUGH_NEXT_FRAME:
		case IDM_FLYTHROUGH_START_POSITION:
		case IDM_FLYTHROUGH_STOP:
		case IDM_FLYTHROUGH_SET_FRAME:
		case IDM_FLYTHROUGH_SET_SPEED:
			success = HandleFlythroughMode(message, fparams, uparams);
			break;

		case IDM_SET_TIME_E3D:
		case IDM_SET_START_TIME_TRAJECTORY:
			success = SetUnixTime(message, uparams.at(0));
			break;
		case IDM_MESH_SET_COLOR:
		case IDM_MESH_SET_ISSEEN:
			success = TransformMeshElement(message, uparams.at(0), fparams);
			break;

		case IDM_MESH_GROUP_SCALE:
		case IDM_MESH_GROUP_ROTATION:
		case IDM_MESH_GROUP_TRANSLATION:
			success = TransformMeshGroup(message, fparams);
			break;

		case IDM_TRAJECTORY_ROTATION:
		case IDM_TRAJECTORY_TRANSLATION:
		case IDM_TRAJECTORY_SET_ISSEEN:
			success = TransformTrajectory(message, fparams);
			break;

		case IDM_ORIGO_SET_LONGITUDE:
		case IDM_ORIGO_SET_LATITUDE:
			success = SetLongitudeLatitude(message, fparams);
			break;
			
		case IDM_CLEAR_TRAJECTORY:
			success = ClearCameraTrajectory();
			break;

		case IDM_CLEAR_MESHES:
			success = ClearMeshes();
			break;

		case IDM_XZ_PLANE_GRID_SET_ISSEEN:
		{
			m_linelist.SetIsSeen(uparams.at(0));
			break;
		}
		case IDM_PIXELSHADER_SET_SHADING:
		{
			m_pixelShaderMesh->SetIsShadingOn(uparams.at(0));
			break;
		}
		default:
			THROW_TREXCEPTION(L"Failed to translate message because it is not implemented");
			break;
		}
		PublishModelState();
		return success;
	}
	catch (const COMException& e)
	{
		ErrorHandler::Log(e);
	}
	catch (const TRException& e)
	{
		ErrorHandler::Log(e);
	}
	catch (const std::exception& e)
	{
		ErrorHandler::Log(e);
	}
	catch (...)
	{
		ErrorHandler::Log("Unknown Exceptio: No details available");
	}
	return false;
}


bool TerrainModel::HandleFlythroughMode(IModelMessageIDs message, const std::vector<float>& fparams, const std::vector<unsigned>& uparams)
{
	bool success = true;
	switch (message)
	{
	case IDM_FLYTHROUGH_START_POSITION:
	case IDM_FLYTHROUGH_STOP:
	{
		m_cameraTrajectory.ResetStartPosition();
		break;
	}
	case IDM_FLYTHROUGH_NEXT_FRAME:
	{
		success = m_cameraTrajectory.UpdateCamera(fparams.at(0));
		break;
	}
	case IDM_FLYTHROUGH_SET_FRAME:
	{
		m_cameraTrajectory.SetCurrentFrame(uparams.at(0));
		success = m_cameraTrajectory.UpdateCamera(NULL);
		break;
	}
	case IDM_FLYTHROUGH_SET_SPEED:
		m_cameraTrajectory.SetSpeed(fparams.at(0));
		break;
	}
	this->m_light.UpdateSunPosition(m_cameraTrajectory.GetCurrentEpochTime().getSeconds(), m_llacoordinate.latitude, m_llacoordinate.longitude);
	return success;
}

bool	TerrainModel::HandleExplore3DMode(IModelMessageIDs message, const std::vector<float>& fparams)
{
	bool success = true;
	switch (message)
	{
	case IDM_E3D_MOVE_FORWARD:
	case IDM_E3D_MOVE_BACK:
	case IDM_E3D_MOVE_LEFT:
	case IDM_E3D_MOVE_RIGHT:
	case IDM_E3D_MOVE_UP:
	case IDM_E3D_MOVE_DOWN:
	{
		this->MoveCamera(message, fparams.at(0));
		break;
	}
	case IDM_E3D_ROTATE:
	{
		this->RotateCamera(message, fparams.at(0), fparams.at(1));
		break;
	}
	case IDM_E3D_SET_SPEED:
	{
		this->m_cameraPositioner.SetSpeed(fparams.at(0));
		break;
	}
	case IDM_E3D_SET_ROTATION_SPEED:
	{
		this->m_cameraPositioner.SetRotationSpeed(fparams.at(0));
		break;
	}
	}
	return success;
}

void TerrainModel::MoveCamera(IModelMessageIDs message, float timeElapsed)
{
	switch (message)
	{
	case IDM_E3D_MOVE_FORWARD:
	{
		this->m_cameraPositioner.MoveForward(timeElapsed);
		break;
	}
	case IDM_E3D_MOVE_BACK:
	{
		this->m_cameraPositioner.MoveBack(timeElapsed);
		break;
	}
	case IDM_E3D_MOVE_LEFT:
	{
		this->m_cameraPositioner.MoveLeft(timeElapsed);
		break;
	}
	case IDM_E3D_MOVE_RIGHT:
	{
		this->m_cameraPositioner.MoveRight(timeElapsed);
		break;
	}
	case IDM_E3D_MOVE_UP:
	{
		this->m_cameraPositioner.MoveUp(timeElapsed);
		break;
	}
	case IDM_E3D_MOVE_DOWN:
	{
		this->m_cameraPositioner.MoveDown(timeElapsed);
		break;
	}
	}
}

void TerrainModel::RotateCamera(IModelMessageIDs message, float pitch, float yaw)
{
	switch (message)
	{
	case IDM_E3D_ROTATE:

		this->m_cameraPositioner.RotatePitchYaw(pitch, yaw);
		break;
	}
}

bool TerrainModel::LoadTerrain_withSoftEdges(const std::wstring& filepath)
{
	VertexMesh* pVertices;
	UINT									vertexCount;
	UINT									indexCount;
	std::vector<VertexMesh>					verticesMesh;
	std::vector<unsigned long>				indices;
	unsigned long* pIndices;

	m_persistence->LoadTerrain_withSoftEdges(filepath.c_str());
	const std::vector<StlVertex>& vertices = m_persistence->GetVertices_Soft();
	const std::vector<CornerIndices>& facetIndices = m_persistence->GetIndices_Soft();
	for (const CornerIndices& facet : facetIndices)
	{
		for (int i = 0; i < 3; i++)
		{
			const unsigned long& index = facet.corner[2 - i];
			indices.push_back(index);
		}
	}
	for (const StlVertex& v : vertices)
	{
		VertexMesh vertexMesh;
		vertexMesh.normal = { (float)v.normal.x, (float)v.normal.z,(float)v.normal.y };
		vertexMesh.position = { (float)v.pos.x, (float)v.pos.z, (float)v.pos.y };
		verticesMesh.push_back(vertexMesh);
	}

	pVertices = &verticesMesh.at(0);
	vertexCount = verticesMesh.size();
	pIndices = &indices.at(0);
	indexCount = indices.size();

	PolygonMeshCreator creator;
	this->m_meshes.Add(pVertices, pIndices, vertexCount, indexCount, creator, StringConverter::GetFileNameFromPath(filepath));

	return true;

}

bool TerrainModel::LoadTerrain_withSharpEdges(const std::wstring& filepath)
{
	VertexMesh*								pVertices;
	UINT									vertexCount;
	UINT									indexCount;
	std::vector<VertexMesh>					vertices;
	std::vector<unsigned long>				indices;
	unsigned long*							pIndices;


	m_persistence->LoadTerrain_withSharpEdges(filepath.c_str());
	const std::vector<stlFacet>& facets = m_persistence->GetFacets();
	unsigned index = 0;
	for (const stlFacet& facet : facets)
	{
		for (int i = 0; i < 3; i++)
		{
			VertexMesh vertex;
			vertex.normal = { (float)facet.normal[0], (float)facet.normal[2],(float)facet.normal[1] };
			vertex.position = { (float)facet.position[2 - i][0], (float)facet.position[2 - i][2], (float)facet.position[2 - i][1] };

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
	this->m_meshes.Add(pVertices, pIndices, vertexCount, indexCount, creator, StringConverter::GetFileNameFromPath(filepath));
	return true;
}

bool	TerrainModel::LoadProject_withSharpEdges(const std::vector<std::wstring>& files)
{
	for (const std::wstring& filepath : files)
	{
		try
		{
			this->LoadTerrain_withSharpEdges(filepath.c_str());
		}
		catch (const COMException& e)
		{
			ErrorHandler::Log(e);
		}
		catch (const TRException& e)
		{
			ErrorHandler::Log(e);
		}
		catch (const std::exception& e)
		{
			ErrorHandler::Log(e);
		}
		catch (...)
		{
			ErrorHandler::Log("Unknown Exceptio: No details available");
		}
	}
	return true;

}

bool	TerrainModel::LoadProject_withSoftEdges(const std::vector<std::wstring>& files)
{
	for (const std::wstring& filepath : files)
	{
		try
		{
			this->LoadTerrain_withSoftEdges(filepath.c_str());
		}
		catch (const COMException& e)
		{
			ErrorHandler::Log(e);
		}
		catch (const TRException& e)
		{
			ErrorHandler::Log(e);
		}
		catch (const std::exception& e)
		{
			ErrorHandler::Log(e);
		}
		catch (...)
		{
			ErrorHandler::Log("Unknown Exception: No details available");
		}
	}
	return true;

}

bool	TerrainModel::LoadConfigurationFile(const std::wstring& filepath)
{
	ParameterFile params;

	m_persistence->LoadConfigurationFile(filepath.c_str(), params);
	//Set world origo
	this->m_llacoordinate = params.origo;
	
	//Set Trajectory
	const float t_x = params.trajectory.translation.x;
	const float t_y = params.trajectory.translation.y;
	const float t_z = params.trajectory.translation.z;

	const float r_x = params.trajectory.rotation.x;
	const float r_y = params.trajectory.rotation.y;
	const float r_z = params.trajectory.rotation.z;

	if (m_cameraTrajectory.IsInitialized())
	{
		this->m_cameraTrajectory.GetPolyLine()->Translate(t_x, t_y, t_z);
		this->m_cameraTrajectory.GetPolyLine()->Rotate(r_x, r_y, r_z);
	}

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
	return true;
}

bool	TerrainModel::LoadCameraTrajectory(const std::wstring& filepath)
{
	std::vector<CameraPose> cameraPoses;

	m_persistence->LoadCameraTrajectory(filepath.c_str(), cameraPoses);
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
	
	m_polylines.Add(&vertices.at(0), NULL, vertices.size(), NULL, creator, StringConverter::GetFileNameFromPath(filepath));
	IRendarablePtr<VertexPolyLine> polyline = m_polylines.GetLastAddedComponent();
	m_cameraTrajectory.Initialize(cameraPoses, polyline, m_camera);
	PublishModelState();
	return true;


}

bool TerrainModel::IsTrajectoryInitialized(void) const
{
	return m_cameraTrajectory.IsInitialized();
}
bool TerrainModel::ResetCamera()
{
	this->m_camera->Reset();
	return true;
}

LLACoordinate	TerrainModel::GetOrigo(void) const
{
	return m_llacoordinate;
}

bool TerrainModel::TransformTrajectory(IModelMessageIDs message, const std::vector<float>& fparams)
{
	switch (message)
	{
	case IDM_TRAJECTORY_SET_ISSEEN:
	{
		bool isSeen = fparams.at(0) > 0 ? 1 : 0;
		m_polylines.SetIsSeen(isSeen);
		break;
	}
	case IDM_TRAJECTORY_ROTATION:
		m_polylines.Rotate(fparams.at(0), fparams.at(1), fparams.at(2));
		break;
	case IDM_TRAJECTORY_TRANSLATION:
		m_polylines.Translate(fparams.at(0), fparams.at(1), fparams.at(2));
		break;
	}
	return true;
}

bool TerrainModel::TransformMeshGroup(IModelMessageIDs message, const std::vector<float>& fparams)
{
	switch (message)
	{
	case IDM_MESH_GROUP_ROTATION:
		m_meshes.Rotate(fparams.at(0), fparams.at(1), fparams.at(2));
		break;
	case IDM_MESH_GROUP_SCALE:
		m_meshes.Scale(fparams.at(0), fparams.at(0), fparams.at(0));
		break;
	case IDM_MESH_GROUP_TRANSLATION:
		m_meshes.Translate(fparams.at(0), fparams.at(1), fparams.at(2));
		break;
	}
	return true;
}

bool TerrainModel::TransformMeshElement(IModelMessageIDs message, unsigned id, const std::vector<float>& fparams)
{
	switch (message)
	{
	case IDM_MESH_SET_COLOR:
	{
		m_meshes.SetColorComponent(id, fparams.at(0), fparams.at(1), fparams.at(2), fparams.at(3));
		break;
	}

	case IDM_MESH_SET_ISSEEN:
	{
		bool m_isSeen = fparams.at(0) > 0 ? 1 : 0;
		m_meshes.SetIsSeenComponent(id, m_isSeen);
		break;
	}
	}
	return true;

}

bool TerrainModel::SetCameraProperties(IModelMessageIDs message, float data)
{
	switch (message)
	{
	case IDM_SET_CAMERA_FIELD_OF_VIEW:
	{
		this->m_camera->SetFieldOfView(data);
		break;
	}
	case IDM_SET_CAMERA_NEAR_SCREEN:
	{
		this->m_camera->SetNearScreen(data);
		break;
	}
	case IDM_SET_CAMERA_FAR_SCREEN:
	{
		this->m_camera->SetFarScreen(data);
		break;
	}
	}
	return true;
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
		state.startEpochTime				= m_cameraTrajectory.GetStartEpochTime();
		state.currentCameraPosition			= m_camera->GetPositionVec();
		state.currentCameraRotation			= m_camera->GetRotationVec();
		state.currentSunPosition.azimuth	= m_light.GetAzimuth();
		state.currentSunPosition.elevation	= m_light.GetElevation();

		TrajectoryState ts;
		const IRenderableState& rs = m_cameraTrajectory.GetTrajectoryPolyLineState();
		ts.id = rs.id;
		ts.m_isSeen = rs.m_isSeen;
		ts.name = rs.name;
		ts.rotation = rs.rotation;
		ts.translation = rs.translation;

		state.trajectoryPolyLine.push_back(ts);
		state.origo = this->m_llacoordinate;
	}
	return state;
}

GeneralModelState TerrainModel::CollectCameraState(void) const
{
	return GeneralModelState{ m_camera->GetFOVrad(), m_camera->GetNearScreen(), m_camera->GetFarScreen(), m_linelist.IsSeen(), m_pixelShaderMesh->GetIsShadingOn()};
}
Explore3DState TerrainModel::CollectExplore3DState(void) const
{
	Explore3DState state;
	state.speed							= m_cameraPositioner.GetSpeed();
	state.rotationSpeed					= m_cameraPositioner.GetRotationSpeed();
	state.currentEpochTime				= m_cameraPositioner.GetCurrentEpochTime();
	state.currentCameraPosition			= m_camera->GetPositionVec();
	state.currentCameraRotation			= m_camera->GetRotationVec();
	state.currentSunPosition.azimuth	= m_light.GetAzimuth();
	state.currentSunPosition.elevation	= m_light.GetElevation();
	state.origo = this->m_llacoordinate;

	return state;
}
MeshGroupState TerrainModel::CollectMeshGroupState() const
{
	MeshGroupState					state;
	std::vector<IRenderableState>	iRenderableStates;
	IRenderableState				compositeIrenderableState;

	m_meshes.CollectIRenderableState(iRenderableStates);
	compositeIrenderableState = m_meshes.GetState();

	state.rotation		= compositeIrenderableState.rotation;
	state.scale			= compositeIrenderableState.scale;
	state.translation	= compositeIrenderableState.translation;

	for (const IRenderableState& irs : iRenderableStates)
	{
		state.Meshes.push_back({ irs.id, irs.name, irs.m_isSeen, irs.color });
	}
	return state;
}

bool TerrainModel::ClearMeshes(void)
{
	this->m_meshes.ClearRenderables();
	return true;
}

bool TerrainModel::ClearCameraTrajectory(void)
{
	if (this->m_cameraTrajectory.GetPolyLine() != nullptr)
	{
		m_polylines.Remove(this->m_cameraTrajectory.GetPolyLine());
		this->m_cameraTrajectory.GetPolyLine()->Shutdown();
	}
	this->m_cameraTrajectory.Clear();
	return true;
}

void TerrainModel::PublishModelState(void) const
{
	this->m_modelMessageSystem.PublishModelState(CollectMeshGroupState());
	this->m_modelMessageSystem.PublishModelState(CollectFlythroughState());
	this->m_modelMessageSystem.PublishModelState(CollectExplore3DState());
	this->m_modelMessageSystem.PublishModelState(CollectCameraState());
}

bool TerrainModel::SetUnixTime(IModelMessageIDs message, unsigned uparam)
{
	switch (message)
	{
	case IDM_SET_TIME_E3D:
		this->m_cameraPositioner.SetCurrentEpochTime({ uparam,0 });
		this->m_light.UpdateSunPosition(m_cameraPositioner.GetCurrentEpochTime().getSeconds(), m_llacoordinate.latitude, m_llacoordinate.longitude);
		break;
	case IDM_SET_START_TIME_TRAJECTORY:
		this->m_cameraTrajectory.SetStartEpochTime({ uparam,0 });
		this->m_light.UpdateSunPosition(m_cameraTrajectory.GetCurrentEpochTime().getSeconds(), m_llacoordinate.latitude, m_llacoordinate.longitude);
	default:
		break;
	}
	return true;
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
		float zCoord = size / 2;
		float xCoord = (-size / 2) + i * gridXStep;
		if (xCoord == 0)
		{
			polygon.color = { 1.0f, 0.0f, 0.0f, 1.0f };
		}
		else
		{
			polygon.color = color;
		}
		polygon.position = { xCoord, 0.0f, zCoord };
		vertices.push_back(polygon);

		polygon.position = { xCoord, 0.0f, -zCoord };
		vertices.push_back(polygon);
	}

	for (int i = 0; i <= gridZ; i++)
	{
		VertexPolyLine polygon;
		float xCoord = size / 2;
		float zCoord = (-size / 2) + i * gridZStep;

		if (zCoord == 0)
		{
			polygon.color = { 0.0f, 1.0f, 0.0f, 1.0f };
		}
		else
			polygon.color = color;


		polygon.position = { xCoord, 0.0f, zCoord };
		vertices.push_back(polygon);

		polygon.position = { -xCoord, 0.0f, zCoord };
		vertices.push_back(polygon);
	}

	VertexPolyLine* pVertex = &vertices[0];
	unsigned verteCount = vertices.size();
	LineListCreator	lineListCreator;
	this->m_linelist.Add(pVertex, NULL, verteCount,NULL, lineListCreator, L"Grid");
}

bool TerrainModel::SetLongitudeLatitude(IModelMessageIDs message, const std::vector<float>& fparams)
{
	switch (message)
	{
	case IDM_ORIGO_SET_LONGITUDE:
		m_llacoordinate.longitude = fparams.at(0);
		break;
	case IDM_ORIGO_SET_LATITUDE:
		m_llacoordinate.latitude = fparams.at(0);
		break;
	
	default:
		break;
	}
	return true;
}




