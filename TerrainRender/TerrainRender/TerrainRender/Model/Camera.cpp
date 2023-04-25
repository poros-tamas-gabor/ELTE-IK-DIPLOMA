#include "Camera.h"
#include <cmath>
#include "../TRException.h"
Camera::Camera()
{
	this->_positionX = START_POSITION.x;
	this->_positionY = START_POSITION.y;
	this->_positionZ = START_POSITION.z;

	this->_rotationX = 0.0f;
	this->_rotationY = 0.0f;
	this->_rotationZ = 0.0f;

	this->_rotationMatrix = DirectX::XMMatrixIdentity();

	Reset();

}
Camera::~Camera() {}

void Camera::SetPosition(float x, float y, float z)
{
	this->_positionX = x;
	this->_positionY = y;
	this->_positionZ = z;
}
void Camera::SetRotationRad(float x, float y, float z)
{
	this->_rotationX = x;
	this->_rotationY = y;
	this->_rotationZ = z;
}

void Camera::Reset()
{
	this->_positionX = START_POSITION.x;
	this->_positionY = START_POSITION.y;
	this->_positionZ = START_POSITION.z;

	this->SetLookAtPos(ORIGO);
}

DirectX::XMFLOAT3 Camera::GetPositionF3(void) const
{
	DirectX::XMFLOAT3 position = { this->_positionX, this->_positionY, this->_positionZ };
	return position;
}
DirectX::XMFLOAT3 Camera::GetRotationF3(void) const
{
	DirectX::XMFLOAT3 rotation = { this->_rotationX, this->_rotationY, this->_rotationZ };
	return rotation;
}

Vector3D Camera::GetPositionVec(void) const
{
	Vector3D position = { this->_positionX, this->_positionY, this->_positionZ };
	return position;
}
Vector3D Camera::GetRotationVec(void) const
{
	Vector3D rotation = { this->_rotationX, this->_rotationY, this->_rotationZ };
	return rotation;
}
void Camera::AdjustPosition(float deltaX, float deltaY, float deltaZ)
{
	this->_positionX += deltaX;
	this->_positionY += deltaY;
	this->_positionZ += deltaZ;
}

void Camera::AdjustRotationRad(float deltaX, float deltaY, float deltaZ)
{
	this->_rotationX += deltaX;
	this->_rotationY += deltaY;
	this->_rotationZ += deltaZ;
}

void Camera::Initialize(int screenWidth, int screenHeight, float screenNear, float screenDepth, float fieldOfView)
{
	THROW_TREXCEPTION_IF_FAILED(screenWidth > 0,	L"Failed to set screenWidth with 0 or negative");
	THROW_TREXCEPTION_IF_FAILED(screenHeight > 0,	L"Failed to set screenHeight with 0 or negative");
	THROW_TREXCEPTION_IF_FAILED(screenNear > 0,		L"Failed to set screenNear with 0 or negative");
	THROW_TREXCEPTION_IF_FAILED(screenDepth > 0,	L"Failed to set screenDepth with 0 or negative");
	THROW_TREXCEPTION_IF_FAILED(fieldOfView > 0,	L"Failed to set fieldOfView with 0 or negative");
	THROW_TREXCEPTION_IF_FAILED((screenDepth - screenNear > 0.1f),	L"Error: ScreenDepth is less than the screenNear");

	m_isInitialized = true;
	float aspectRatio = (float)screenWidth / (float)screenHeight;
	this->SetProjectionValues(fieldOfView, aspectRatio, screenNear, screenDepth);
}

void Camera::Resize(int screenWidth, int screenHeight)
{
	THROW_TREXCEPTION_IF_FAILED(screenWidth > 0, L"Failed to set screenWidth with 0 or negative");
	THROW_TREXCEPTION_IF_FAILED(screenHeight > 0, L"Failed to set screenHeight with 0 or negative");
	if (m_isInitialized)
	{
		this->m_aspectRatio = (float)screenWidth / (float)screenHeight;
		this->_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(m_fovRadian, m_aspectRatio, m_nearScreen, m_farScreen);
	}
}


void Camera::SetProjectionValues(float fovRadian, float aspectRatio, float screenNear, float screenDepth)
{
	THROW_TREXCEPTION_IF_FAILED(fovRadian > 0,	L"Failed to set fovRadian with 0 or negative");
	THROW_TREXCEPTION_IF_FAILED(aspectRatio > 0,L"Failed to set aspectRatio with 0 or negative");
	THROW_TREXCEPTION_IF_FAILED(screenNear > 0,	L"Failed to set nearScreen with 0 or negative");
	THROW_TREXCEPTION_IF_FAILED(screenDepth > 0,	L"Failed to set farScreen with 0 or negative");
	THROW_TREXCEPTION_IF_FAILED((screenDepth - screenNear > 0.1f), L"Error: ScreenDepth is less than the screenNear");
	this->m_fovRadian = fovRadian;
	this->m_aspectRatio = aspectRatio;
	this->m_nearScreen = screenNear;
	this->m_farScreen = screenDepth;
	this->_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadian, aspectRatio, screenNear, screenDepth);
}

void Camera::SetFieldOfView(float fovRad)
{
	THROW_TREXCEPTION_IF_FAILED(fovRad > 0, L"Failed to set fovRad with 0 or negative");
	this->m_fovRadian = fovRad;
	this->_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(m_fovRadian, m_aspectRatio, m_nearScreen, m_farScreen);
}
void Camera::SetNearScreen(float nearZ)
{
	THROW_TREXCEPTION_IF_FAILED(nearZ > 0, L"Failed to set nearZ with 0 or negative");
	THROW_TREXCEPTION_IF_FAILED((m_farScreen - nearZ > 0.1f), L"Error: ScreenDepth is less than the screenNear");
	this->m_nearScreen = nearZ;
	this->_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(m_fovRadian, m_aspectRatio, m_nearScreen, m_farScreen);
}
void Camera::SetFarScreen(float farZ)
{
	THROW_TREXCEPTION_IF_FAILED(farZ > 0, L"Failed to set farZ with 0 or negative");
	THROW_TREXCEPTION_IF_FAILED((farZ - m_nearScreen > 0.1f), L"Error: ScreenDepth is less than the screenNear");
	this->m_farScreen = farZ;
	this->_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(m_fovRadian, m_aspectRatio, m_nearScreen, m_farScreen);
}

void Camera::SetLookAtPos(const DirectX::XMFLOAT3& lookAtFloat)
{
	//Verify that look at pos is not the same as cam pos. They cannot be the same as that wouldn't make sense and would result in undefined behavior.
	if (lookAtFloat.x == this->_positionX && lookAtFloat.y == this->_positionY && lookAtFloat.z == this->_positionZ)
		return;

		
	// Move the lookAt vector with the EyePosition vector
	DirectX::XMFLOAT3 lookAt = { this->_positionX - lookAtFloat.x, this->_positionY - lookAtFloat.y , this->_positionZ - lookAtFloat.z };

	float pitch = 0.0f;
	if (lookAt.y != 0.0f)
	{
		const float distance = std::sqrt(lookAt.x * lookAt.x + lookAt.z * lookAt.z);
		pitch = std::atan(lookAt.y / distance);
	}

	float yaw = 0.0f;
	if (lookAt.x != 0.0f)
	{
		yaw = std::atan(lookAt.x / lookAt.z);
	}
	if (lookAt.z > 0)
		yaw += DirectX::XM_PI;

	this->SetRotationRad(pitch, yaw, 0.0f);

}



void Camera::Render()
{
	float rotXRad = this->_rotationX;
	float rotYRad = this->_rotationY;
	float rotZRad = this->_rotationZ;
	// Create the rotation matrix from the yaw, pitch, and roll values.
	this->_rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotXRad, rotYRad, rotZRad);

	// Setup the position of the camera in the world.
	DirectX::XMVECTOR positonVector = DirectX::XMVectorSet(this->_positionX, this->_positionY, this->_positionZ, 0.0f);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	DirectX::XMVECTOR lookatVector = DirectX::XMVector4Transform(this->DEFAULT_LOOKAT_VECTOR, this->_rotationMatrix);
	// Translate the rotated camera position to the location of the viewer.
	lookatVector = DirectX::XMVectorAdd(lookatVector, positonVector);

	DirectX::XMVECTOR upVector = DirectX::XMVector4Transform(this->DEFAULT_UP_VECTOR, this->_rotationMatrix);

	this->_viewMatrix = DirectX::XMMatrixLookAtLH(positonVector, lookatVector, upVector);

}
DirectX::XMMATRIX Camera::GetViewMatrix(void) const
{
	return this->_viewMatrix;
}

DirectX::XMMATRIX Camera::GetProjectionMatrix(void) const
{
	return this->_projectionMatrix;
}

DirectX::XMMATRIX Camera::GetRotationMatrix(void) const
{
	return this->_rotationMatrix;
}

float Camera::GetFOVrad(void) const
{
	return this->m_fovRadian;
}
float Camera::GetNearScreen(void) const
{
	return this->m_nearScreen;
}
float Camera::GetFarScreen(void) const
{
	return this->m_farScreen;
}