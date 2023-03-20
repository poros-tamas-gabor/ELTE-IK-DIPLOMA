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

	bresult = this->m_vertexShader.Initialize(device, hwnd);
	if (!bresult)
	{
		return false;
	}

	bresult = this->m_vertexShaderPolygon.Initialize(device, hwnd);
	if (!bresult)
	{
		return false;
	}

	bresult = this->m_pixelShader.Initialize(device, hwnd);
	if (!bresult)
	{
		return false;
	}

	this->m_camera.Initialize(screenWidth, screenHeight, screenNear, screenDepth, fieldOfView);
	this->m_position.Initialize(&this->m_camera);

	this->m_meshes.Initialize(device, &m_vertexShader, &m_pixelShader, NULL, NULL);


	this->m_polygons.Initialize(device, &m_vertexShaderPolygon, &m_pixelShader, NULL, NULL);
	this->AddGrid(2000, { 1.0f, 1.0f, 1.0f, 1.0f }, 200, 200);

	return true;
}

void TerrainModel::Shutdown()
{
	m_vertexShader.Shutdown();
	m_pixelShader.Shutdown();
	m_meshes.Shutdown();
	m_polygons.Shutdown();
}

bool TerrainModel::Render(ID3D11DeviceContext* deviceContext)
{
	this->m_camera.Render();

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();

	this->m_meshes.Render(deviceContext,worldMatrix, m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix(), m_light);

	this->m_polygons.Render(deviceContext, worldMatrix, m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix(), m_light);

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
	Vertex*				pVertices;
	UINT				vertexCount;
	std::vector<Vertex> vertices;
	bool bresult = m_persistence->LoadTerrain(filepath, vertices);
	if (bresult)
	{
		pVertices = &vertices.at(0);
		vertexCount = vertices.size();
		this->m_meshes.Add(pVertices, vertexCount, m_polygonMeshCreator);
	}

	return bresult;
}

bool	TerrainModel::LoadCameraTrajectory(const wchar_t* filepath)
{
	std::vector<CameraPose> cameraPoses;
	bool result = m_persistence->LoadCameraTrajectory(filepath, cameraPoses);
	if (result)
	{
		PolyLine* polyline = new PolyLine;
		std::vector<VertexPolygon> vertices;
		for (const CameraPose& camerapose : cameraPoses)
		{
			VertexPolygon vertex;
			vertex.position = { (float)camerapose.east,-(float)camerapose.down,(float)camerapose.north };
			vertex.color = { 1.0f, 0.0f, 1.0f, 1.0f };
			vertices.push_back(vertex);
		}
		polyline->Initialize(this->m_device, &this->m_vertexShaderPolygon, &this->m_pixelShader, &vertices.at(0), vertices.size());

		m_polygons.Add(polyline);
		m_cameraTrajectory.Initialize(cameraPoses, polyline, &m_camera);
	}
	return result;
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
	std::vector<VertexPolygon> vertices;
	if (size <= 0 || gridX <= 0 || gridZ <= 0)
		return;

	float gridXStep = size / gridX;
	float gridZStep = size / gridZ;

	for (int i = 0; i <= gridX; i++)
	{
		VertexPolygon polygon;
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
		VertexPolygon polygon;
		polygon.color = color;
		float xCoord = size / 2;
		float zCoord = (-size / 2) + i * gridZStep;
		polygon.position = { xCoord, 0.0f, zCoord };
		vertices.push_back(polygon);

		polygon.position = { -xCoord, 0.0f, zCoord };
		vertices.push_back(polygon);
	}

	VertexPolygon* pVertex = &vertices[0];
	unsigned verteCount = vertices.size();
	this->m_polygons.Add(pVertex, verteCount, m_polygonCreator);
}


