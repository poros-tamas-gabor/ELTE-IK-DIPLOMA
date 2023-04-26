#ifndef CAMERA_H
#define CAMERA_H

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

	DirectX::XMMATRIX	_viewMatrix;
	DirectX::XMMATRIX	_projectionMatrix;
	DirectX::XMMATRIX	_rotationMatrix;

	

	float	_positionX;
	float	_positionY;
	float	_positionZ;

	float	_rotationX;		//in radian
	float	_rotationY;		//in radian
	float	_rotationZ;		//in radian

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

	void Render();
	DirectX::XMMATRIX GetViewMatrix(void) const;
	DirectX::XMMATRIX GetProjectionMatrix(void) const;
	DirectX::XMMATRIX GetRotationMatrix(void) const;

	float GetFOVrad(void) const;
	float GetNearScreen(void) const;
	float GetFarScreen(void) const;
};
typedef std::shared_ptr<Camera> CameraPtr;
#endif // !CAMERA_H
