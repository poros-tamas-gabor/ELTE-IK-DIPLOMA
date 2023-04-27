#include "CameraPositioner.h"
#include "Camera.h"
#include "../TRException.h"
void CameraPositioner::MoveTemplate(const DirectX::XMVECTOR& default_vector, DirectX::XMVECTOR& currentVector, float dt)
{
	this->m_rotationMatrix = this->m_camera->GetRotationMatrix();
	currentVector = DirectX::XMVector3Transform(default_vector, this->m_rotationMatrix);
	currentVector = DirectX::XMVectorScale(currentVector, this->m_speed * dt);
	DirectX::XMFLOAT3 float3;
	DirectX::XMStoreFloat3(&float3, currentVector);
	this->m_camera->AdjustPosition(float3.x, float3.y, float3.z);
}

void CameraPositioner::RotatePitchYaw(float x, float y)
{
	x *= this->m_rotationSpeed;
	y *= this->m_rotationSpeed;
	this->m_camera->AdjustRotationRad(x, y, 0.0f);
}

void CameraPositioner::SetSpeed(float speed)
{
	this->m_speed = speed;
}
void CameraPositioner::SetRotationSpeed(float speed)
{
	this->m_rotationSpeed = speed;
}

float CameraPositioner::GetSpeed() const
{
	return this->m_speed;
}
float CameraPositioner::GetRotationSpeed() const
{
	return this->m_rotationSpeed;
}

void CameraPositioner::MoveForward(float dt)
{
	this->MoveTemplate(this->DEFAULT_FORWARD, this->m_current_forward, dt);
}
void CameraPositioner::MoveBack(float dt)
{
	this->MoveTemplate(this->DEFAULT_BACK, this->m_current_back, dt);
}
void CameraPositioner::MoveLeft(float dt)
{
	this->MoveTemplate(this->DEFAULT_LEFT, this->m_current_left, dt);
}
void CameraPositioner::MoveRight(float dt)
{
	this->MoveTemplate(this->DEFAULT_RIGHT, this->m_current_right, dt);
}

void CameraPositioner::MoveUp(float dt)
{
	this->MoveTemplate(this->DEFAULT_UP, this->m_current_up, dt);
}

void CameraPositioner::MoveDown(float dt)
{
	this->MoveTemplate(this->DEFAULT_DOWN, this->m_current_down, dt);
}

void CameraPositioner::Initialize(CameraPtr camera)
{
	THROW_TREXCEPTION_IF_FAILED( (camera != nullptr) , L"Null pointer exception");
	this->m_camera = camera;
	this->m_rotationMatrix = this->m_camera->GetRotationMatrix();
}


EpochTime CameraPositioner::GetCurrentEpochTime(void) const
{
	return m_currentEpochTime;
}
void CameraPositioner::SetCurrentEpochTime(EpochTime epochtime)
{
	m_currentEpochTime = epochtime;
}