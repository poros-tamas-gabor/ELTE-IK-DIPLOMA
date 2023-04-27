#include "pch.h"

TEST(CameraUnitTest, Initialize)
{
	Camera camera;
	ASSERT_THROW(camera.Initialize(0, 1, 1, 2, 1), TRException);
	ASSERT_THROW(camera.Initialize(1, 0, 1, 2, 1), TRException);
	ASSERT_THROW(camera.Initialize(1, 1, 0, 2, 1), TRException);
	ASSERT_THROW(camera.Initialize(1, 1, 1, 0, 1), TRException);
	ASSERT_THROW(camera.Initialize(1, 1, 1, 2, 0), TRException);
	ASSERT_THROW(camera.Initialize(1, 1, 1, 1, 1), TRException);

	camera.Initialize(800, 600, 0.1f, 100.0f, DirectX::XMConvertToRadians(60.0f));
	
	DirectX::XMMATRIX actual = camera.GetProjectionMatrix();
	DirectX::XMMATRIX expected = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), float(800) / 600, 0.1f, 100.0f);

	ASSERT_TRUE(AreMatricesEqual(actual, expected, 0.0001f));
}

TEST(CameraUnitTest, Resize)
{
	Camera camera;
	camera.Initialize(800, 600, 0.1f, 100.0f, DirectX::XMConvertToRadians(60.0f));

	ASSERT_THROW(camera.Resize(0, 0), TRException);

	camera.Resize(1024, 768);
	DirectX::XMMATRIX actual = camera.GetProjectionMatrix();
	DirectX::XMMATRIX expected = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), float(1024) / 768, 0.1f, 100.0f);

	ASSERT_TRUE(AreMatricesEqual(actual, expected, 0.0001f));
}

TEST(CameraUnitTest, SetPosition)
{
	Camera camera;
	camera.SetPosition(1.0f, 2.0f, 3.0f);
	DirectX::XMFLOAT3 positionF = camera.GetPositionF3();
	Vector3D positionV = camera.GetPositionVec();
	ASSERT_FLOAT_EQ(positionF.x, 1.0f);
	ASSERT_FLOAT_EQ(positionF.y, 2.0f);
	ASSERT_FLOAT_EQ(positionF.z, 3.0f);
	ASSERT_EQ(positionV, Vector3D(1.0f, 2.0f, 3.0f));
}

TEST(CameraUnitTest, SetRotation)
{
	Camera camera;
	camera.SetRotationRad(1.0f, 2.0f, 3.0f);
	DirectX::XMFLOAT3 rotationF = camera.GetRotationF3();
	Vector3D rotationV = camera.GetRotationVec();
	ASSERT_FLOAT_EQ(rotationF.x, 1.0f);
	ASSERT_FLOAT_EQ(rotationF.y, 2.0f);
	ASSERT_FLOAT_EQ(rotationF.z, 3.0f);
	ASSERT_EQ(rotationV, Vector3D(1.0f, 2.0f, 3.0f));
}

TEST(CameraUnitTest, AdjustPosition)
{
	Camera				camera;
	Vector3D			expected;
	DirectX::XMFLOAT3	positionF;
	Vector3D			positionV;

	camera.SetPosition(0.0f, 0.0f, 0.0f);

	camera.AdjustPosition(1.0f, 2.0f, 3.0f);
	expected = { 1.0f, 2.0f, 3.0f };
	positionF = camera.GetPositionF3();
	positionV = camera.GetPositionVec();
	ASSERT_FLOAT_EQ(positionF.x, expected.x);
	ASSERT_FLOAT_EQ(positionF.y, expected.y);
	ASSERT_FLOAT_EQ(positionF.z, expected.z);
	ASSERT_EQ(positionV, expected);

	camera.AdjustPosition(-3.0f, +7.0f, 11.0f);
	expected = { 1.0f - 3.0f, 2.0f + 7.0f, 3.0f + 11.0f };
	positionF = camera.GetPositionF3();
	positionV = camera.GetPositionVec();
	ASSERT_FLOAT_EQ(positionF.x, expected.x);
	ASSERT_FLOAT_EQ(positionF.y, expected.y);
	ASSERT_FLOAT_EQ(positionF.z, expected.z);
	ASSERT_EQ(positionV, expected);

	camera.AdjustPosition(+13.0f, +17.0f, -23.0f);
	expected = { 1.0f - 3.0f + 13.0f, 2.0f + 7.0f + 17.0f, 3.0f + 11.0f -23.0f };
	positionF = camera.GetPositionF3();
	positionV = camera.GetPositionVec();
	ASSERT_FLOAT_EQ(positionF.x, expected.x);
	ASSERT_FLOAT_EQ(positionF.y, expected.y);
	ASSERT_FLOAT_EQ(positionF.z, expected.z);
	ASSERT_EQ(positionV, expected);
}

TEST(CameraUnitTest, AdjustRotation)
{
	Camera				camera;
	Vector3D			expected;
	DirectX::XMFLOAT3	rotationF;
	Vector3D			rotationV;

	camera.SetRotationRad(0.0f, 0.0f, 0.0f);

	camera.AdjustRotationRad(1.0f, 2.0f, 3.0f);
	expected = { 1.0f, 2.0f, 3.0f };
	rotationF = camera.GetRotationF3();
	rotationV = camera.GetRotationVec();
	ASSERT_FLOAT_EQ(rotationF.x, expected.x);
	ASSERT_FLOAT_EQ(rotationF.y, expected.y);
	ASSERT_FLOAT_EQ(rotationF.z, expected.z);
	ASSERT_EQ(rotationV, expected);

	camera.AdjustRotationRad(-3.0f, +7.0f, 11.0f);
	expected = { 1.0f - 3.0f, 2.0f + 7.0f, 3.0f + 11.0f };
	rotationF = camera.GetRotationF3();
	rotationV = camera.GetRotationVec();
	ASSERT_FLOAT_EQ(rotationF.x, expected.x);
	ASSERT_FLOAT_EQ(rotationF.y, expected.y);
	ASSERT_FLOAT_EQ(rotationF.z, expected.z);
	ASSERT_EQ(rotationV, expected);

	camera.AdjustRotationRad(+13.0f, +17.0f, -23.0f);
	expected = { 1.0f - 3.0f + 13.0f, 2.0f + 7.0f + 17.0f, 3.0f + 11.0f - 23.0f };
	rotationF = camera.GetRotationF3();
	rotationV = camera.GetRotationVec();
	ASSERT_FLOAT_EQ(rotationF.x, expected.x);
	ASSERT_FLOAT_EQ(rotationF.y, expected.y);
	ASSERT_FLOAT_EQ(rotationF.z, expected.z);
	ASSERT_EQ(rotationV, expected);
}

TEST(CameraUnitTest, SetNearScreen)
{
	Camera camera;
	camera.Initialize(800, 600, 0.1f, 100.0f, DirectX::XMConvertToRadians(60.0f));

	ASSERT_THROW(camera.SetNearScreen(0.0f), TRException);
	ASSERT_THROW(camera.SetNearScreen(100.0f), TRException);
	
	camera.SetNearScreen(1.0f);

	DirectX::XMMATRIX actual = camera.GetProjectionMatrix();
	DirectX::XMMATRIX expected = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), float(800) / 600, 1.0f, 100.0f);

	ASSERT_TRUE(AreMatricesEqual(actual, expected, 0.0001f));
}

TEST(CameraUnitTest, SetFarScreen)
{
	Camera camera;
	camera.Initialize(800, 600, 0.1f, 100.0f, DirectX::XMConvertToRadians(60.0f));

	ASSERT_THROW(camera.SetFarScreen(0.0f), TRException);
	ASSERT_THROW(camera.SetFarScreen(0.1f), TRException);

	camera.SetFarScreen(200.0f);

	DirectX::XMMATRIX actual = camera.GetProjectionMatrix();
	DirectX::XMMATRIX expected = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), float(800) / 600, 0.1f, 200.0f);

	ASSERT_TRUE(AreMatricesEqual(actual, expected, 0.0001f));
}

TEST(CameraUnitTest, Render)
{
	// Create a new camera
	Camera camera;

	// Set the camera's position and rotation
	camera.SetPosition(0.0f, 0.0f, -5.0f);
	camera.SetRotationRad(0.0f, 0.0f, 0.0f);

	// Render the camera
	camera.Render();

	// Verify that the view matrix is correct
	DirectX::XMMATRIX expectedViewMatrix = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	);
	DirectX::XMMATRIX viewMatrix = camera.GetViewMatrix();

	ASSERT_TRUE(AreMatricesEqual(expectedViewMatrix, viewMatrix, 0.0001f));
}



