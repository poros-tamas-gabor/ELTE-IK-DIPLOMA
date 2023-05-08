#include "Mocks.h"
#include "pch.h"

using ::testing::Return;

class PolyLineUnitTest : public ::testing::Test
{
protected:
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	IVertexShaderPtr m_vertexShader;
	IPixelShaderPtr m_pixelShader;

	unsigned _vertexCount = 6;
	unsigned _indexCount = 6;
	VertexPolyLine m_vertices[6] = { VertexPolyLine(),VertexPolyLine(),VertexPolyLine(),VertexPolyLine() };
	unsigned long m_indices[6] = { 0,1,2,3,4,5 };

	virtual void SetUp()
	{
		//m_device = std::make_shared<MockID3D11Device>();

		m_vertexShader = std::make_shared<MockIVertexShader>();
		m_pixelShader = std::make_shared<MockIPixelShader>();

		HRESULT hr = D3D11CreateDevice(
			nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &m_device, nullptr, nullptr);
		ASSERT_TRUE(SUCCEEDED(hr)) << "Failed to create D3D11 device";
	}
};

TEST_F(PolyLineUnitTest, Test_InitializeReturns)
{
	PolyLine polyline;
	ASSERT_TRUE(polyline.Initialize(m_device, m_vertexShader, m_pixelShader, m_vertices, m_indices, _vertexCount, _indexCount));
}
TEST_F(PolyLineUnitTest, Test_InitializeReturnsFalseWithNullVertexShader)
{

	PolyLine polyline;
	bool result = polyline.Initialize(m_device, nullptr, m_pixelShader, m_vertices, m_indices, _vertexCount, _indexCount);
	EXPECT_FALSE(result);
}

// Test case to check if the Initialize method returns false when the device parameter is null
TEST_F(PolyLineUnitTest, Test_InitializeReturnsFalseWithNullDevice)
{
	PolyLine polyline;
	bool result = polyline.Initialize(nullptr, m_vertexShader, m_pixelShader, m_vertices, m_indices, _vertexCount, _indexCount);
	EXPECT_FALSE(result);
}

// Test case to check if the Initialize method returns false when the vertex shader parameter is null
TEST_F(PolyLineUnitTest, Test_InitializeReturnsFalseWithNullPixelShader)
{
	PolyLine polyline;

	bool result = polyline.Initialize(m_device, m_vertexShader, nullptr, m_vertices, m_indices, _vertexCount, _indexCount);

	EXPECT_FALSE(result);
}

TEST_F(PolyLineUnitTest, Test_SetNameAndGetName)
{
	PolyLine polyline;
	ASSERT_TRUE(polyline.Initialize(m_device, m_vertexShader, m_pixelShader, m_vertices, m_indices, _vertexCount, _indexCount));
	polyline.SetName(L"TestMesh");
	ASSERT_EQ(polyline.GetName(), L"TestMesh");
	ASSERT_EQ(polyline.GetState().name, L"TestMesh");

}

TEST_F(PolyLineUnitTest, Test_Rotate)
{
	PolyLine			polyline;
	Vector3D			actual;

	ASSERT_TRUE(polyline.Initialize(m_device, m_vertexShader, m_pixelShader, m_vertices, m_indices, _vertexCount, _indexCount));


	actual = polyline.GetState().rotation;
	ASSERT_EQ(actual, Vector3D(0, 0, 0));

	polyline.Rotate(PI, PI / 2, -PI / 4);
	actual = polyline.GetState().rotation;
	ASSERT_EQ(actual, Vector3D(PI, PI / 2, -PI / 4));

	polyline.Rotate(1.0f, 1.0f, 1.0f);
	actual = polyline.GetState().rotation;
	ASSERT_EQ(actual, Vector3D(1.0f, 1.0f, 1.0f));

	polyline.ResetTransformation();
	actual = polyline.GetState().rotation;
	ASSERT_EQ(actual, Vector3D(0, 0, 0));
}

TEST_F(PolyLineUnitTest, Test_Scale)
{
	PolyLine			polyline;
	Vector3D			actual;

	ASSERT_TRUE(polyline.Initialize(m_device, m_vertexShader, m_pixelShader, m_vertices, m_indices, _vertexCount, _indexCount));


	actual = polyline.GetState().scale;
	ASSERT_EQ(actual, Vector3D(1.0f, 1.0f, 1.0f));


	polyline.Scale(PI, PI / 2, -PI / 4);
	actual = polyline.GetState().scale;
	ASSERT_EQ(actual, Vector3D(1.0f, 1.0f, 1.0f));
	//ASSERT_THROW(polyline.Scale(PI, PI / 2, -PI / 4), TRException);

	polyline.ResetTransformation();
	actual = polyline.GetState().scale;
	ASSERT_EQ(actual, Vector3D(1.0f, 1.0f, 1.0f));
}

TEST_F(PolyLineUnitTest, Test_Translate)
{
	PolyLine			polyline;
	Vector3D			actual;

	ASSERT_TRUE(polyline.Initialize(m_device, m_vertexShader, m_pixelShader, m_vertices, m_indices, _vertexCount, _indexCount));


	actual = polyline.GetState().translation;
	ASSERT_EQ(actual, Vector3D(0, 0, 0));

	polyline.Translate(PI, PI / 2, -PI / 4);
	actual = polyline.GetState().translation;
	ASSERT_EQ(actual, Vector3D(PI, PI / 2, -PI / 4));

	polyline.Translate(1.0f, 1.0f, 1.0f);
	actual = polyline.GetState().translation;
	ASSERT_EQ(actual, Vector3D(1.0f, 1.0f, 1.0f));

	polyline.ResetTransformation();
	actual = polyline.GetState().translation;
	ASSERT_EQ(actual, Vector3D(0, 0, 0));
}


TEST_F(PolyLineUnitTest, Test_SetColor)
{
	PolyLine polyline;
	polyline.SetColor(1.0f, 0.5f, 0.0f, 1.0f);

	Vector4D	expected = { 1.0f, 0.5f, 0.0f, 1.0f };
	Vector4D	actual = polyline.GetState().color;
	ASSERT_EQ(expected, actual);
}

TEST_F(PolyLineUnitTest, Test_SetIsSeenAndIsSeen)
{
	PolyLine polyline;
	polyline.SetIsSeen(true);
	ASSERT_TRUE(polyline.IsSeen());
	ASSERT_TRUE(polyline.GetState().m_isSeen);
	polyline.SetIsSeen(false);
	ASSERT_FALSE(polyline.IsSeen());
	ASSERT_FALSE(polyline.GetState().m_isSeen);
}

