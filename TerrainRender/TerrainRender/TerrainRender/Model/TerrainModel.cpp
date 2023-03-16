#include "TerrainModel.h"

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

void TerrainModel::OnMoveCamera(const CameraMoveEvent::Event& event)
{
	float timeElapsed = event.GetData();
	if (event.IsType(CameraMoveEvent::Type::MoveForward))
	{
		this->m_position.MoveForward(timeElapsed);
	}
	else if (event.IsType(CameraMoveEvent::Type::MoveBack))
	{
		this->m_position.MoveBack(timeElapsed);
	}
	else if (event.IsType(CameraMoveEvent::Type::MoveLeft))
	{
		this->m_position.MoveLeft(timeElapsed);
	}
	else if (event.IsType(CameraMoveEvent::Type::MoveRight))
	{
		this->m_position.MoveRight(timeElapsed);
	}
	else if (event.IsType(CameraMoveEvent::Type::MoveUp))
	{
		this->m_position.MoveUp(timeElapsed);
	}
	else if (event.IsType(CameraMoveEvent::Type::MoveDown))
	{
		this->m_position.MoveDown(timeElapsed);
	}
}

void TerrainModel::OnRotateCamera(const CameraRotateEvent::Event& event)
{
	
	CameraRotateEvent::CameraRotation cameraRotation = event.GetData();
	this->m_position.RotatePitchYaw(cameraRotation.pitch, cameraRotation.yaw);
	
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
		this->m_scene.Add(pVertices, vertexCount);
	}
	return bresult;
}
