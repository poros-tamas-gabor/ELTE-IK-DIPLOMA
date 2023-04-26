#include "pch.h"

TEST(CameraPositionerUnitTest, Initialize)
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