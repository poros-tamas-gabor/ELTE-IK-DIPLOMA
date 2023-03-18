#include "TerrainModel.h"
#include "../resource.h"
TerrainModel::TerrainModel() = default;
TerrainModel::~TerrainModel() = default;


bool TerrainModel::Initalize(HWND hwnd, IDataAccess* persistence, ID3D11Device* device, int screenWidth, int screenHeight, float screenNear, float screenDepth, float fieldOfView)
{
	bool bresult;

	if (persistence == nullptr || device == nullptr)
		return false;

	this->m_persistence = persistence;

	bresult = this->m_vertexShader.Initialize(device, hwnd);
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

	this->m_scene.Initialize(device, &m_vertexShader, &m_pixelShader, NULL, NULL);

	return true;
}

void TerrainModel::Shutdown()
{
	m_vertexShader.Shutdown();
	m_pixelShader.Shutdown();
	m_scene.Shutdown();
}

bool TerrainModel::Render(ID3D11DeviceContext* deviceContext)
{
	this->m_camera.Render();

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();

	this->m_scene.Render(deviceContext,worldMatrix, m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix(), m_light);

	return true;
}


void TerrainModel::MoveCamera(unsigned message, float timeElapsed)
{
	switch (message)
	{
	case IDC_CAMERA_MOVE_FORWARD:
	{
		this->m_position.MoveForward(timeElapsed);
		break;
	}
	case IDC_CAMERA_MOVE_BACK:
	{
		this->m_position.MoveBack(timeElapsed);
		break;
	}
	case IDC_CAMERA_MOVE_LEFT:
	{
		this->m_position.MoveLeft(timeElapsed);
		break;
	}
	case IDC_CAMERA_MOVE_RIGHT:
	{
		this->m_position.MoveRight(timeElapsed);
		break;
	}
	case IDC_CAMERA_MOVE_UP:
	{
		this->m_position.MoveUp(timeElapsed);
		break;
	}
	case IDC_CAMERA_MOVE_DOWN:
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
	case IDC_CAMERA_ROTATE:

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
		this->m_scene.Add(pVertices, vertexCount, m_polygonMeshCreator);
	}

	return bresult;
}
void TerrainModel::ResetCamera()
{
	this->m_camera.SetLookAtPos({ 0,0,0 });
}
void TerrainModel::UpdateCameraProperties(unsigned message, float data)
{
	switch (message)
	{
	case IDC_SET_CAMERA_SPEED:
	{
		this->m_position.SetSpeed(data);
		break;
	}
	case IDC_SET_CAMERA_ROTATION_SPEED:
	{
		this->m_position.SetRotationSpeed(data);
	}

	case IDC_SET_CAMERA_FIELD_OF_VIEW:
	{
		//this->m_position.SetRotationSpeed(data);
	}
	case IDC_SET_CAMERA_ASPECT_RATIO:
	{
		//this->m_position.SetRotationSpeed(data);
	}
	case IDC_SET_CAMERA_ASPECT_NEAR_SCREEN:
	{
		//this->m_position.SetRotationSpeed(data);
	}
	case IDC_SET_CAMERA_ASPECT_FAR_SCREEN:
	{
		//this->m_position.SetRotationSpeed(data);
	}
	default:
		break;
	}
}




