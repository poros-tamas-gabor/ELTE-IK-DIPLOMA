#include "Camera.h"
#include <cmath>
#include "../TRException.h"
Camera::Camera()
{
	m_positionX = START_POSITION.x;
	m_positionY = START_POSITION.y;
	m_positionZ = START_POSITION.z;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_rotationMatrix = DirectX::XMMatrixIdentity();

	Reset();

}
Camera::~Camera() {}

void Camera::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
}
void Camera::SetRotationRad(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
}

void Camera::Reset()
{
	m_positionX = START_POSITION.x;
	m_positionY = START_POSITION.y;
	m_positionZ = START_POSITION.z;

	SetLookAtPos(ORIGO);
}

DirectX::XMFLOAT3 Camera::GetPositionF3(void) const
{
	DirectX::XMFLOAT3 position = { m_positionX, m_positionY, m_positionZ };
	return position;
}
DirectX::XMFLOAT3 Camera::GetRotationF3(void) const
{
	DirectX::XMFLOAT3 rotation = { m_rotationX, m_rotationY, m_rotationZ };
	return rotation;
}
Vector3D Camera::GetPositionVec(void) const
{
	Vector3D position = { m_positionX, m_positionY, m_positionZ };
	return position;
}
Vector3D Camera::GetRotationVec(void) const
{
	Vector3D rotation = { m_rotationX, m_rotationY, m_rotationZ };
	return rotation;
}

void Camera::AdjustPosition(float deltaX, float deltaY, float deltaZ)
{
	m_positionX += deltaX;
	m_positionY += deltaY;
	m_positionZ += deltaZ;
}
void Camera::AdjustRotationRad(float deltaX, float deltaY, float deltaZ)
{
	m_rotationX += deltaX;
	m_rotationY += deltaY;
	m_rotationZ += deltaZ;
}

void Camera::Initialize(int screenWidth, int screenHeight, float screenNear, float screenDepth, float fieldOfView)
{
	THROW_TREXCEPTION_IF_FAILED((screenWidth > 0),	L"Failed to set screenWidth with 0 or negative");
	THROW_TREXCEPTION_IF_FAILED((screenHeight > 0),	L"Failed to set screenHeight with 0 or negative");
	THROW_TREXCEPTION_IF_FAILED((screenNear > 0),	L"Failed to set screenNear with 0 or negative");
	THROW_TREXCEPTION_IF_FAILED((screenDepth > 0),	L"Failed to set screenDepth with 0 or negative");
	THROW_TREXCEPTION_IF_FAILED((fieldOfView > 0),	L"Failed to set fieldOfView with 0 or negative");
	THROW_TREXCEPTION_IF_FAILED((screenDepth - screenNear > 0.1f),	L"Error: ScreenDepth is less than the screenNear");

	m_isInitialized = true;
	float aspectRatio = (float)screenWidth / (float)screenHeight;
	SetProjectionValues(fieldOfView, aspectRatio, screenNear, screenDepth);
}

void Camera::Resize(int screenWidth, int screenHeight)
{
	THROW_TREXCEPTION_IF_FAILED((screenWidth > 0), L"Failed to set screenWidth with 0 or negative");
	THROW_TREXCEPTION_IF_FAILED((screenHeight > 0), L"Failed to set screenHeight with 0 or negative");
	if (m_isInitialized)
	{
		m_aspectRatio = (float)screenWidth / (float)screenHeight;
		m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(m_fovRadian, m_aspectRatio, m_nearScreen, m_farScreen);
	}
}


void Camera::SetProjectionValues(float fovRadian, float aspectRatio, float screenNear, float screenDepth)
{
	THROW_TREXCEPTION_IF_FAILED((fovRadian > 0),	L"Failed to set fovRadian with 0 or negative");
	THROW_TREXCEPTION_IF_FAILED((aspectRatio > 0),	L"Failed to set aspectRatio with 0 or negative");
	THROW_TREXCEPTION_IF_FAILED((screenNear > 0),	L"Failed to set nearScreen with 0 or negative");
	THROW_TREXCEPTION_IF_FAILED((screenDepth > 0),	L"Failed to set farScreen with 0 or negative");
	THROW_TREXCEPTION_IF_FAILED((screenDepth - screenNear > 0.1f), L"Error: ScreenDepth is less than the screenNear");
	m_fovRadian = fovRadian;
	m_aspectRatio = aspectRatio;
	m_nearScreen = screenNear;
	m_farScreen = screenDepth;
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadian, aspectRatio, screenNear, screenDepth);
}

void Camera::SetFieldOfView(float fovRad)
{
	THROW_TREXCEPTION_IF_FAILED((fovRad > 0), L"Failed to set fovRad with 0 or negative");
	m_fovRadian = fovRad;
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(m_fovRadian, m_aspectRatio, m_nearScreen, m_farScreen);
}
void Camera::SetNearScreen(float nearZ)
{
	THROW_TREXCEPTION_IF_FAILED((nearZ > 0), L"Failed to set nearZ with 0 or negative");
	THROW_TREXCEPTION_IF_FAILED((m_farScreen - nearZ > 0.1f), L"Error: ScreenDepth is less than the screenNear");
	m_nearScreen = nearZ;
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(m_fovRadian, m_aspectRatio, m_nearScreen, m_farScreen);
}
void Camera::SetFarScreen(float farZ)
{
	THROW_TREXCEPTION_IF_FAILED((farZ > 0), L"Failed to set farZ with 0 or negative");
	THROW_TREXCEPTION_IF_FAILED((farZ - m_nearScreen > 0.1f), L"Error: ScreenDepth is less than the screenNear");
	m_farScreen = farZ;
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(m_fovRadian, m_aspectRatio, m_nearScreen, m_farScreen);
}

void Camera::SetLookAtPos(const DirectX::XMFLOAT3& lookAtFloat)
{
	//Verify that look at pos is not the same as cam pos. They cannot be the same as that wouldn't make sense and would result in undefined behavior.
	if (lookAtFloat.x == m_positionX && lookAtFloat.y == m_positionY && lookAtFloat.z == m_positionZ)
		return;

		
	// Move the lookAt vector with the EyePosition vector
	DirectX::XMFLOAT3 lookAt = { m_positionX - lookAtFloat.x, m_positionY - lookAtFloat.y , m_positionZ - lookAtFloat.z };

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

	SetRotationRad(pitch, yaw, 0.0f);

}



void Camera::Render()
{
	float rotXRad = m_rotationX;
	float rotYRad = m_rotationY;
	float rotZRad = m_rotationZ;
	// Create the rotation matrix from the yaw, pitch, and roll values.
	m_rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotXRad, rotYRad, rotZRad);

	// Setup the position of the camera in the world.
	DirectX::XMVECTOR positonVector = DirectX::XMVectorSet(m_positionX, m_positionY, m_positionZ, 0.0f);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	DirectX::XMVECTOR lookatVector = DirectX::XMVector4Transform(DEFAULT_LOOKAT_VECTOR, m_rotationMatrix);
	// Translate the rotated camera position to the location of the viewer.
	lookatVector = DirectX::XMVectorAdd(lookatVector, positonVector);

	DirectX::XMVECTOR upVector = DirectX::XMVector4Transform(DEFAULT_UP_VECTOR, m_rotationMatrix);

	m_viewMatrix = DirectX::XMMatrixLookAtLH(positonVector, lookatVector, upVector);

}
DirectX::XMMATRIX Camera::GetViewMatrix(void) const
{
	return m_viewMatrix;
}

DirectX::XMMATRIX Camera::GetProjectionMatrix(void) const
{
	return m_projectionMatrix;
}

DirectX::XMMATRIX Camera::GetRotationMatrix(void) const
{
	return m_rotationMatrix;
}

float Camera::GetFOVrad(void) const
{
	return m_fovRadian;
}
float Camera::GetNearScreen(void) const
{
	return m_nearScreen;
}
float Camera::GetFarScreen(void) const
{
	return m_farScreen;
}