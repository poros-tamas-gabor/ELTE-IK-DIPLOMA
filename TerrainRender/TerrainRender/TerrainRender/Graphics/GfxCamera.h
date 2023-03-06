#ifndef GFX_CAMERA_H
#define GFX_CAMERA_H

#include <DirectXMath.h>
class GfxCamera 
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
	GfxCamera();
	GfxCamera(const GfxCamera& other) = delete;
	~GfxCamera();

	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

	void SetPosition(float x, float y, float z);
	void SetRotationRad(float x, float y, float z);
	void AdjustPosition(float deltaX, float deltaY, float deltaZ);
	void AdjustRotationRad(float deltaX, float deltaY, float deltaZ);

	void SetLookAtPos(const DirectX::XMFLOAT3& lookAt);

	DirectX::XMVECTOR GetPosition(void);
	DirectX::XMVECTOR GetRotationRad(void);

	void Render();
	void GetViewMatrix(DirectX::XMMATRIX& viewMatrix);
	void GetProjectionMatrix(DirectX::XMMATRIX& projectionMatrix);
	void GetRotationMatrix(DirectX::XMMATRIX& roationMatrix);


};
#endif // !GFX_CAMERA_H
