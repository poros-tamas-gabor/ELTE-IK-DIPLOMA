#include "Position.h"
#include "Camera.h"
void Position::SetVectors(const DirectX::XMMATRIX rotationMatrix)
{
	this->_rotationMatrix = rotationMatrix;
}

void Position::MoveTemplate(const DirectX::XMVECTOR& default_vector, DirectX::XMVECTOR& currentVector, float dt)
{
	currentVector = DirectX::XMVector3Transform(default_vector, this->_rotationMatrix);
	currentVector = DirectX::XMVectorScale(currentVector, this->m_speed * dt);
	DirectX::XMFLOAT3 float3;
	DirectX::XMStoreFloat3(&float3, currentVector);
	this->m_camera->AdjustPosition(float3.x, float3.y, float3.z);
}

void Position::RotatePitchYaw(float x, float y)
{
	x *= this->m_rotationSpeed;
	y *= this->m_rotationSpeed;
	this->m_camera->AdjustRotationRad(x, y, 0.0f);
}

void Position::SetSpeed(float speed)
{
	this->m_speed = speed;
}
void Position::SetRotationSpeed(float speed)
{
	this->m_rotationSpeed = speed;
}

void Position::MoveForward(float dt)
{
	this->_rotationMatrix = this->m_camera->GetRotationMatrix();
	this->MoveTemplate(this->DEFAULT_FORWARD, this->m_current_forward, dt);
}
void Position::MoveBack(float dt)
{
	this->_rotationMatrix = this->m_camera->GetRotationMatrix();
	this->MoveTemplate(this->DEFAULT_BACK, this->m_current_back, dt);
}
void Position::MoveLeft(float dt)
{
	this->_rotationMatrix = this->m_camera->GetRotationMatrix();
	this->MoveTemplate(this->DEFAULT_LEFT, this->m_current_left, dt);
}
void Position::MoveRight(float dt)
{
	this->_rotationMatrix = this->m_camera->GetRotationMatrix();
	this->MoveTemplate(this->DEFAULT_RIGHT, this->m_current_right, dt);
}
void Position::MoveUp(float dt)
{
	this->_rotationMatrix = this->m_camera->GetRotationMatrix();
	this->MoveTemplate(this->DEFAULT_UP, this->m_current_up, dt);
}
void Position::MoveDown(float dt)
{
	this->_rotationMatrix = this->m_camera->GetRotationMatrix();
	this->MoveTemplate(this->DEFAULT_DOWN, this->m_current_down, dt);
}

void Position::Initialize(Camera* camera)
{
	this->m_camera = camera;
	this->_rotationMatrix = this->m_camera->GetRotationMatrix();
}
