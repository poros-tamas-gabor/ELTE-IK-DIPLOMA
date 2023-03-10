#ifndef POSITION_H
#define	POSITION_H

#include <DirectXMath.h>

class GfxCamera;
class Position 
{

private:
	const DirectX::XMVECTOR DEFAULT_FORWARD = { 0.0f , 0.0f, 1.0f, 0.0f };
	const DirectX::XMVECTOR DEFAULT_BACK = { 0.0f , 0.0f, -1.0f, 0.0f };
	const DirectX::XMVECTOR DEFAULT_UP = { 0.0f , 1.0f, 0.0f, 0.0f };
	const DirectX::XMVECTOR DEFAULT_DOWN = { 0.0f , -1.0f, 0.0f, 0.0f };
	const DirectX::XMVECTOR DEFAULT_LEFT = { -1.0f , 0.0f, 0.0f, 0.0f };
	const DirectX::XMVECTOR DEFAULT_RIGHT = { 1.0f , 0.0f, 0.0f, 0.0f };

	DirectX::XMVECTOR _current_forward;
	DirectX::XMVECTOR _current_back;
	DirectX::XMVECTOR _current_up;
	DirectX::XMVECTOR _current_down;
	DirectX::XMVECTOR _current_left;
	DirectX::XMVECTOR _current_right;

	DirectX::XMMATRIX _rotationMatrix;

	GfxCamera* _camera;

	float _speed = 0.005f;
	float _rotationSpeed = 0.001f;

	void MoveTemplate(const DirectX::XMVECTOR& default_vector, DirectX::XMVECTOR& currentVector, float dt);
public:

	void SetVectors(const DirectX::XMMATRIX rotationMatrix);
	void MoveForward(float dt);
	void MoveBack(float dt);
	void MoveLeft(float dt);
	void MoveRight(float dt);
	void MoveUp(float dt);
	void MoveDown(float dt);
	void RotatePitchYaw(float x, float y);
	void SetCamera(GfxCamera* camera);


};

#endif