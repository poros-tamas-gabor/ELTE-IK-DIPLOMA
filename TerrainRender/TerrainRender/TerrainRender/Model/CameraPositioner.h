#ifndef CAMERA_POSITIONER_H
#define	CAMERA_POSITIONER_H

#include <DirectXMath.h>
#include "Persistence/ModelStructs.h"
#include <ctime>
#include <memory>
class Camera;
typedef std::shared_ptr<Camera> CameraPtr;
class CameraPositioner
{

private:
	const DirectX::XMVECTOR DEFAULT_FORWARD = { 0.0f , 0.0f, 1.0f, 0.0f };
	const DirectX::XMVECTOR DEFAULT_BACK = { 0.0f , 0.0f, -1.0f, 0.0f };
	const DirectX::XMVECTOR DEFAULT_UP = { 0.0f , 1.0f, 0.0f, 0.0f };
	const DirectX::XMVECTOR DEFAULT_DOWN = { 0.0f , -1.0f, 0.0f, 0.0f };
	const DirectX::XMVECTOR DEFAULT_LEFT = { -1.0f , 0.0f, 0.0f, 0.0f };
	const DirectX::XMVECTOR DEFAULT_RIGHT = { 1.0f , 0.0f, 0.0f, 0.0f };

	DirectX::XMVECTOR m_current_forward;
	DirectX::XMVECTOR m_current_back;
	DirectX::XMVECTOR m_current_up;
	DirectX::XMVECTOR m_current_down;
	DirectX::XMVECTOR m_current_left;
	DirectX::XMVECTOR m_current_right;

	DirectX::XMMATRIX m_rotationMatrix = DirectX::XMMatrixIdentity();

	CameraPtr m_camera;

	float m_speed = 0.005f;
	float m_rotationSpeed = 0.001f;
	EpochTime	m_currentEpochTime = EpochTime(std::time(NULL), 0);

	void MoveTemplate(const DirectX::XMVECTOR& default_vector, DirectX::XMVECTOR& currentVector, float dt);
public:

	void MoveForward(float dt);
	void MoveBack(float dt);
	void MoveLeft(float dt);
	void MoveRight(float dt);
	void MoveUp(float dt);
	void MoveDown(float dt);
	void RotatePitchYaw(float x, float y);
	void Initialize(CameraPtr camera);

	void SetSpeed(float speed);
	void SetRotationSpeed(float speed);

	float GetSpeed() const;
	float GetRotationSpeed() const;

	EpochTime GetCurrentEpochTime(void) const;
	void SetCurrentEpochTime(EpochTime);

};

#endif