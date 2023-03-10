#include "Position.h"
#include "GfxCamera.h"
void Position::SetVectors(const DirectX::XMMATRIX rotationMatrix)
{
	this->_rotationMatrix = rotationMatrix;
}

void Position::MoveTemplate(const DirectX::XMVECTOR& default_vector, DirectX::XMVECTOR& currentVector, float dt)
{
	currentVector = DirectX::XMVector3Transform(default_vector, this->_rotationMatrix);
	currentVector = DirectX::XMVectorScale(currentVector, this->_speed * dt);
	DirectX::XMFLOAT3 float3;
	DirectX::XMStoreFloat3(&float3, currentVector);
	this->_camera->AdjustPosition(float3.x, float3.y, float3.z);
}

void Position::RotatePitchYaw(float x, float y)
{
	x *= this->_rotationSpeed;
	y *= this->_rotationSpeed;
	this->_camera->AdjustRotationRad(x, y, 0.0f);
}


void Position::MoveForward(float dt)
{
	this->_camera->GetRotationMatrix(this->_rotationMatrix);
	this->MoveTemplate(this->DEFAULT_FORWARD, this->_current_forward, dt);
}
void Position::MoveBack(float dt)
{
	this->_camera->GetRotationMatrix(this->_rotationMatrix);
	this->MoveTemplate(this->DEFAULT_BACK, this->_current_back, dt);
}
void Position::MoveLeft(float dt)
{
	this->_camera->GetRotationMatrix(this->_rotationMatrix);
	this->MoveTemplate(this->DEFAULT_LEFT, this->_current_left, dt);
}
void Position::MoveRight(float dt)
{
	this->_camera->GetRotationMatrix(this->_rotationMatrix);
	this->MoveTemplate(this->DEFAULT_RIGHT, this->_current_right, dt);
}

void Position::SetCamera(GfxCamera* camera)
{
	this->_camera = camera;
	this->_camera->GetRotationMatrix(this->_rotationMatrix);
}