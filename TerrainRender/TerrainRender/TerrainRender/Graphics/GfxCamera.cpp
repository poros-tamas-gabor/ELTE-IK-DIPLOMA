#include "GfxCamera.h"

GfxCamera::GfxCamera() = default;
GfxCamera::~GfxCamera() {}

void GfxCamera::SetPosition(float x, float y, float z)
{
	this->_positionX = x;
	this->_positionY = y;
	this->_positionZ = z;
}
void GfxCamera::SetRotationRad(float x, float y, float z)
{
	this->_rotationX = x;
	this->_rotationY = y;
	this->_rotationZ = z;
}

DirectX::XMVECTOR GfxCamera::GetPosition(void)
{
	DirectX::XMFLOAT3 position = { this->_positionX, this->_positionY, this->_positionZ };
	DirectX::XMVECTOR positionVector = DirectX::XMLoadFloat3(&position);
	return positionVector;
}
DirectX::XMVECTOR GfxCamera::GetRotationRad(void)
{
	DirectX::XMFLOAT3 rotation = { this->_rotationX, this->_rotationY, this->_rotationZ };
	DirectX::XMVECTOR rotationVector = DirectX::XMLoadFloat3(&rotation);
	return rotationVector;
}
void GfxCamera::AdjustPosition(float deltaX, float deltaY, float deltaZ)
{
	this->_positionX += deltaX;
	this->_positionY += deltaY;
	this->_positionZ += deltaZ;
}

void GfxCamera::AdjustRotationRad(float deltaX, float deltaY, float deltaZ)
{
	this->_rotationX += deltaX;
	this->_rotationY += deltaY;
	this->_rotationZ += deltaZ;
}

void GfxCamera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearScreen, float farScreen)
{
	this->_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fovDegrees), aspectRatio, nearScreen, farScreen);
}

void GfxCamera::SetLookAtPos(const DirectX::XMFLOAT3& lookAtFloat)
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



void GfxCamera::Render()
{
	float rotXRad = this->_rotationX;
	float rotYRad = this->_rotationY;
	float rotZRad = this->_rotationZ;
	// Create the rotation matrix from the yaw, pitch, and roll values.
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotXRad, rotYRad, rotZRad);

	// Setup the position of the camera in the world.
	DirectX::XMVECTOR positonVector  = DirectX::XMVectorSet(this->_positionX, this->_positionY, this->_positionZ, 0.0f);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	DirectX::XMVECTOR lookatVector = DirectX::XMVector4Transform(this->DEFAULT_LOOKAT_VECTOR, rotationMatrix);
	// Translate the rotated camera position to the location of the viewer.
	lookatVector = DirectX::XMVectorAdd(lookatVector, positonVector);

	DirectX::XMVECTOR upVector = DirectX::XMVector4Transform(this->DEFAULT_UP_VECTOR, rotationMatrix);

	this->_viewMatrix = DirectX::XMMatrixLookAtLH(positonVector, lookatVector, upVector);

}
void GfxCamera::GetViewMatrix(DirectX::XMMATRIX& viewMatrix)
{
	viewMatrix = this->_viewMatrix;
}

void GfxCamera::GetProjectionMatrix(DirectX::XMMATRIX& projectionMatrix)
{
	projectionMatrix = this->_projectionMatrix;
}