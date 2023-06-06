#ifndef CAMERA_H
#define CAMERA_H

///////////////////////////////////////////////////////////////////////////////
// Camera.h
// ========
//
// The Camera class implements the virtual camera of a program.
// Its data members contain the necessary parameters for creating the view and projection matrices.
// These attributes include the position, three Euler angles, vertical field of view, aspect ratio, far and near clipping planes.
// There are appropriate setter methods available for setting these attributes.
// 
// The Camera::Render function calculates the view and projection matrices using the appropriate DirectX system calls.
// 
// AUTHOR: Pindrought
// URL: https://github.com/Pindrought/DirectX-11-Engine-VS2017
// CREATED: Unknown
// MODIFIED: 2023-05-08 by TAMAS GABOR POROS
///////////////////////////////////////////////////////////////////////////////

#include <DirectXMath.h>
#include "Persistence/ModelStructs.h"
#include <memory>
class Camera 
{ 
private:
	const DirectX::XMFLOAT3 START_POSITION = { 20.0f, 40.0f, -50.0f};
	const DirectX::XMFLOAT3 ORIGO = { 0.0f, 0.0f, 0.0f };
	const DirectX::XMVECTOR DEFAULT_LOOKAT_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_UP_VECTOR = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	DirectX::XMMATRIX	m_viewMatrix;
	DirectX::XMMATRIX	m_projectionMatrix;
	DirectX::XMMATRIX	m_rotationMatrix;

	float	m_positionX;
	float	m_positionY;
	float	m_positionZ;

	float	m_rotationX;		//in radian
	float	m_rotationY;		//in radian
	float	m_rotationZ;		//in radian

	float	m_fovRadian, m_aspectRatio, m_nearScreen, m_farScreen;
	bool	m_isInitialized = false;

	void SetProjectionValues(float fovRad, float aspectRatio, float nearZ, float farZ);
	void SetLookAtPos(const DirectX::XMFLOAT3& lookAt);

public:
	Camera();
	Camera(const Camera& other) = delete;
	~Camera();

	void Initialize(int screenWidth, int screenHeight, float screenNear, float screenDepth, float fieldOfView);
	void Resize(int screenWidth, int screenHeight);
	void Reset(void);
	void Render(void);

	void SetFieldOfView(float fovRad);
	void SetNearScreen(float nearZ);
	void SetFarScreen(float farZ);

	void SetPosition(float x, float y, float z);
	void SetRotationRad(float x, float y, float z);
	void AdjustPosition(float deltaX, float deltaY, float deltaZ);
	void AdjustRotationRad(float deltaX, float deltaY, float deltaZ);

	Vector3D GetPositionVec(void) const;
	Vector3D GetRotationVec(void) const;
	DirectX::XMFLOAT3 GetPositionF3(void) const;
	DirectX::XMFLOAT3 GetRotationF3(void) const;

	DirectX::XMMATRIX GetViewMatrix(void) const;
	DirectX::XMMATRIX GetProjectionMatrix(void) const;
	DirectX::XMMATRIX GetRotationMatrix(void) const;

	float GetFOVrad(void) const;
	float GetNearScreen(void) const;
	float GetFarScreen(void) const;
};
typedef std::shared_ptr<Camera> CameraPtr;
#endif // !CAMERA_H
