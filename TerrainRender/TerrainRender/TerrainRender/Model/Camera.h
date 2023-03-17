#ifndef CAMERA_H
#define CAMERA_H

#include <DirectXMath.h>
class Camera 
{ 
private:
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


public:
	Camera();
	Camera(const Camera& other) = delete;
	~Camera();

	void Initialize(int screenWidth, int screenHeight, float screenNear, float screenDepth, float fieldOfView);

	void SetProjectionValues(float fovRad, float aspectRatio, float nearZ, float farZ);

	void SetPosition(float x, float y, float z);
	void SetRotationRad(float x, float y, float z);
	void AdjustPosition(float deltaX, float deltaY, float deltaZ);
	void AdjustRotationRad(float deltaX, float deltaY, float deltaZ);

	void SetLookAtPos(const DirectX::XMFLOAT3& lookAt);

	DirectX::XMVECTOR GetPosition(void);
	DirectX::XMVECTOR GetRotationRad(void);

	void Render();
	DirectX::XMMATRIX GetViewMatrix(void);
	DirectX::XMMATRIX GetProjectionMatrix(void);
	DirectX::XMMATRIX GetRotationMatrix(void);


};
#endif // !CAMERA_H
