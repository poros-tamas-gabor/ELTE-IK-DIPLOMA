#include "TerrainModel.h"
#include "../resource.h"
#include "PolyLine.h"
#include "../ErrorHandler.h"

TerrainModel::TerrainModel() = default;
TerrainModel::~TerrainModel() = default;
const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_HEIGHT = 1024;
const float SHADOWMAP_DEPTH = 50.0f;
const float SHADOWMAP_NEAR = 1.0f;


bool TerrainModel::Initalize(HWND hwnd, IDataAccess* persistence, ID3D11Device* device, int screenWidth, int screenHeight, float screenNear, float screenDepth, float fieldOfView)
{
	bool bresult;

	if (persistence == nullptr || device == nullptr)
		return false;

	this->m_persistence = persistence;

	this->m_device = device;

	bresult = this->m_vertexShaderMesh.Initialize(device, hwnd);
	THROW_TREXCEPTION_IF_FAILED(bresult, L"Failed to Initialize vertex shader for meshes");

	bresult = this->m_vertexShaderPolyLine.Initialize(device, hwnd);
	THROW_TREXCEPTION_IF_FAILED(bresult, L"Failed to Initialize vertex shader for polylines");

	bresult = this->m_pixelShaderMesh.Initialize(device, hwnd);
	THROW_TREXCEPTION_IF_FAILED(bresult, L"Failed to Initialize pixel shader for meshes");

	bresult = this->m_pixelShaderPolyLine.Initialize(device, hwnd);
	THROW_TREXCEPTION_IF_FAILED(bresult, L"Failed to Initialize pixel shader for polylines");

	// Initialize the depth shader object.
	bresult = m_depthPixelShader.Initialize(device, hwnd);
	THROW_TREXCEPTION_IF_FAILED(bresult, L"Failed to initialize the depth shader object.");

	// Initialize the depth shader object.
	bresult = m_depthVertexShader.Initialize(device, hwnd);
	THROW_TREXCEPTION_IF_FAILED(bresult, L"Failed to initialize the depth shader object.");

	this->m_light.Initialize(20.0f, SHADOWMAP_DEPTH, SHADOWMAP_NEAR);

	this->m_camera.Initialize(screenWidth, screenHeight, screenNear, screenDepth, fieldOfView);
	this->m_position.Initialize(&this->m_camera);

	this->m_meshes.Initialize(device, NULL, NULL);
	this->m_polylines.Initialize(device, NULL, NULL);
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
	m_depthPixelShader.Shutdown();
	m_depthVertexShader.Shutdown();
}

bool TerrainModel::RenderShadowMap(ID3D11DeviceContext* deviceContext)
{
	this->m_camera.Render();
	this->m_light.Render();

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();

	this->m_meshes.Render(deviceContext, m_depthVertexShader, m_depthPixelShader, worldMatrix, m_light.GetLightViewMatrix(), m_light.GetLightOrthoProjectionMatrix(), m_light);

	return true;
}
bool TerrainModel::Render(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* depthMapTexture)
{
	this->m_camera.Render();
	this->m_light.Render();

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();

	m_pixelShaderMesh.SetShaderResources(deviceContext, depthMapTexture);
	this->m_meshes.Render(deviceContext,m_vertexShaderMesh, m_pixelShaderMesh, worldMatrix, m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix(), m_light);

	this->m_polylines.Render(deviceContext, m_vertexShaderPolyLine, m_pixelShaderPolyLine, worldMatrix, m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix(), m_light);

	return true;
}

void TerrainModel::Flythrough(unsigned message, double elapsedMillisec)
{
	switch (message)
	{

	case IDM_CAMERA_TRAJECTORY_START:
	case IDM_CAMERA_TRAJECTORY_STOP:
	{
		this->m_cameraTrajectory.Reset();
		break;
	}
	case IDM_CAMERA_TRAJECTORY_NEXT_FRAME:
	{
		this->m_cameraTrajectory.UpdateCamera(elapsedMillisec);
		break;
	}
	}
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


	bool bresult = m_persistence->LoadTerrain(filepath, vertices);
	if (bresult)
	{
		pVertices = &vertices.at(0);
		vertexCount = vertices.size();
		PolygonMeshCreator creator;
		this->m_meshes.Add(pVertices, vertexCount, creator);
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
		PolyLine* polyline = new PolyLine;
		std::vector<VertexPolyLine> vertices;
		for (const CameraPose& camerapose : cameraPoses)
		{
			VertexPolyLine vertex;
			vertex.position = { (float)camerapose.east,-(float)camerapose.down,(float)camerapose.north };
			vertex.color = { 1.0f, 0.0f, 1.0f, 1.0f };
			vertices.push_back(vertex);
		}
		polyline->Initialize(this->m_device, &vertices.at(0), vertices.size());
		m_polylines.Add(polyline);
		m_cameraTrajectory.Initialize(cameraPoses, polyline, &m_camera);
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
	this->m_polylines.Add(pVertex, verteCount, lineListCreator);
}


