#include "Camera.h"

Camera::Camera()
{
	this->_positionX = 0.0f;
	this->_positionY = 2.0f;
	this->_positionZ = -2.0f;

	this->_rotationX = DirectX::XM_PIDIV4;
	this->_rotationY = 0.0f;
	this->_rotationZ = 0.0f;

	this->_rotationMatrix = DirectX::XMMatrixIdentity();

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

DirectX::XMFLOAT3 Camera::GetPositionF3(void)
{
	DirectX::XMFLOAT3 position = { this->_positionX, this->_positionY, this->_positionZ };
	return position;
}
DirectX::XMFLOAT3 Camera::GetRotationF3(void)
{
	DirectX::XMFLOAT3 rotation = { this->_rotationX, this->_rotationY, this->_rotationZ };
	return rotation;
}

DirectX::XMVECTOR Camera::GetPositionVec(void)
{
	DirectX::XMFLOAT3 position = { this->_positionX, this->_positionY, this->_positionZ };
	DirectX::XMVECTOR positionVector = DirectX::XMLoadFloat3(&position);
	return positionVector;
}
DirectX::XMVECTOR Camera::GetRotationVec(void)
{
	DirectX::XMFLOAT3 rotation = { this->_rotationX, this->_rotationY, this->_rotationZ };
	DirectX::XMVECTOR rotationVector = DirectX::XMLoadFloat3(&rotation);
	return rotationVector;
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
	m_isInitialized = true;
	float aspectRatio = (float)screenWidth / (float)screenHeight;
	this->SetProjectionValues(fieldOfView, aspectRatio, screenNear, screenDepth);
}

void Camera::Resize(int screenWidth, int screenHeight)
{
	if (m_isInitialized)
	{
		this->m_aspectRatio = (float)screenWidth / (float)screenHeight;
		this->_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(m_fovRadian, m_aspectRatio, m_nearScreen, m_farScreen);
	}
}


void Camera::SetProjectionValues(float fovRadian, float aspectRatio, float nearScreen, float farScreen)
{
	this->m_fovRadian = fovRadian;
	this->m_aspectRatio = aspectRatio;
	this->m_nearScreen = nearScreen;
	this->m_farScreen = farScreen;
	this->_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadian, aspectRatio, nearScreen, farScreen);
}

void Camera::SetFieldOfView(float fovRad)
{
	this->m_fovRadian = fovRad;
	this->_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(m_fovRadian, m_aspectRatio, m_nearScreen, m_farScreen);
}
void Camera::SetNearScreen(float nearZ)
{
	this->m_nearScreen = nearZ;
	this->_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(m_fovRadian, m_aspectRatio, m_nearScreen, m_farScreen);
}
void Camera::SetFarScreen(float farZ)
{
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
		const float distance = sqrt(lookAt.x * lookAt.x + lookAt.z * lookAt.z);
		pitch = atan(lookAt.y / distance);
	}

	float yaw = 0.0f;
	if (lookAt.x != 0.0f)
	{
		yaw = atan(lookAt.x / lookAt.z);
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
DirectX::XMMATRIX Camera::GetViewMatrix(void)
{
	return this->_viewMatrix;
}

DirectX::XMMATRIX Camera::GetProjectionMatrix(void)
{
	return this->_projectionMatrix;
}

DirectX::XMMATRIX Camera::GetRotationMatrix(void)
{
	return this->_rotationMatrix;
}