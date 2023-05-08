#include "Mocks.h"
#include "pch.h"

using ::testing::Return;

class PolygonMeshUnitTest : public ::testing::Test
{
protected:
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	IVertexShaderPtr m_vertexShader;
	IPixelShaderPtr m_pixelShader;

	unsigned _vertexCount = 4;
	unsigned _indexCount = 6;
	VertexMesh m_vertices[4] = {VertexMesh(),VertexMesh(),VertexMesh(),VertexMesh() };
	unsigned long m_indices[6] = {0,1,2,2,3,4};

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

TEST_F(PolygonMeshUnitTest, Test_Initialize)
{
	PolygonMesh mesh;
	ASSERT_TRUE(mesh.Initialize(m_device, m_vertexShader, m_pixelShader, m_vertices, m_indices, _vertexCount, _indexCount));
}
TEST_F(PolygonMeshUnitTest, Test_InitializeReturnsFalseWithNullVertexShader)
{
	IVertexShaderPtr vertexShader;
	PolygonMesh mesh;
	bool result = mesh.Initialize(m_device, vertexShader, m_pixelShader, m_vertices, m_indices, _vertexCount, _indexCount);
	EXPECT_FALSE(result);
}

// Test case to check if the Initialize method returns false when the device parameter is null
TEST_F(PolygonMeshUnitTest, Test_InitializeReturnsFalseWithNullDevice)
{
	PolygonMesh mesh;
	bool result = mesh.Initialize(nullptr, m_vertexShader, m_pixelShader, m_vertices, m_indices, _vertexCount, _indexCount);
	EXPECT_FALSE(result);
}

// Test case to check if the Initialize method returns false when the vertex shader parameter is null
TEST_F(PolygonMeshUnitTest, Test_InitializeReturnsFalseWithNullPixelShader)
{
	PolygonMesh mesh;

	bool result = mesh.Initialize(m_device, m_vertexShader, nullptr, m_vertices, m_indices, _vertexCount, _indexCount);

	EXPECT_FALSE(result);
}

TEST_F(PolygonMeshUnitTest, Test_SetNameAndGetName)
{
	PolygonMesh mesh;
	ASSERT_TRUE(mesh.Initialize(m_device, m_vertexShader, m_pixelShader, m_vertices, m_indices, _vertexCount, _indexCount));
	mesh.SetName(L"TestMesh");
	ASSERT_EQ(mesh.GetName(), L"TestMesh");
	ASSERT_EQ(mesh.GetState().name, L"TestMesh");

}

TEST_F(PolygonMeshUnitTest, Test_Rotate)
{
	PolygonMesh			mesh;
	Vector3D			actual;

	ASSERT_TRUE(mesh.Initialize(m_device, m_vertexShader, m_pixelShader, m_vertices, m_indices, _vertexCount, _indexCount));


	actual = mesh.GetState().rotation;
	ASSERT_EQ(actual, Vector3D(0, 0, 0));
		
	mesh.Rotate(PI, PI/2, -PI/4);
	actual = mesh.GetState().rotation;
	ASSERT_EQ(actual, Vector3D(PI, PI / 2, -PI / 4));

	mesh.Rotate(1.0f, 1.0f, 1.0f);
	actual = mesh.GetState().rotation;
	ASSERT_EQ(actual, Vector3D(1.0f, 1.0f, 1.0f));

	mesh.ResetTransformation();
	actual = mesh.GetState().rotation;
	ASSERT_EQ(actual, Vector3D(0, 0, 0));
}

TEST_F(PolygonMeshUnitTest, Test_Scale)
{
	PolygonMesh			mesh;
	Vector3D			actual;

	ASSERT_TRUE(mesh.Initialize(m_device, m_vertexShader, m_pixelShader, m_vertices, m_indices, _vertexCount, _indexCount));


	actual = mesh.GetState().scale;
	ASSERT_EQ(actual, Vector3D(1.0f, 1.0f, 1.0f));

	mesh.Scale(PI, PI / 2, -PI / 4);
	actual = mesh.GetState().scale;
	ASSERT_EQ(actual, Vector3D(PI, PI / 2, -PI / 4));

	mesh.Scale(1.0f, 1.0f, 1.0f);
	actual = mesh.GetState().scale;
	ASSERT_EQ(actual, Vector3D(1.0f, 1.0f, 1.0f));

	mesh.ResetTransformation();
	actual = mesh.GetState().scale;
	ASSERT_EQ(actual, Vector3D(1.0f, 1.0f, 1.0f));
}

TEST_F(PolygonMeshUnitTest, Test_Translate)
{
	PolygonMesh			mesh;
	Vector3D			actual;

	ASSERT_TRUE(mesh.Initialize(m_device, m_vertexShader, m_pixelShader, m_vertices, m_indices, _vertexCount, _indexCount));


	actual = mesh.GetState().translation;
	ASSERT_EQ(actual, Vector3D(0, 0, 0));

	mesh.Translate(PI, PI / 2, -PI / 4);
	actual = mesh.GetState().translation;
	ASSERT_EQ(actual, Vector3D(PI, PI / 2, -PI / 4));

	mesh.Translate(1.0f, 1.0f, 1.0f);
	actual = mesh.GetState().translation;
	ASSERT_EQ(actual, Vector3D(1.0f, 1.0f, 1.0f));

	mesh.ResetTransformation();
	actual = mesh.GetState().translation;
	ASSERT_EQ(actual, Vector3D(0, 0, 0));
}


TEST_F(PolygonMeshUnitTest, Test_SetColor)
{
	PolygonMesh mesh;
	mesh.SetColor(1.0f, 0.5f, 0.0f, 1.0f);

	Vector4D	expected = { 1.0f, 0.5f, 0.0f, 1.0f };
	Vector4D	actual = mesh.GetState().color;
	ASSERT_EQ(expected, actual);
}

TEST_F(PolygonMeshUnitTest, Test_SetIsSeenAndIsSeen)
{
	PolygonMesh mesh;
	mesh.SetIsSeen(true);
	ASSERT_TRUE(mesh.IsSeen());
	ASSERT_TRUE(mesh.GetState().m_isSeen);
	mesh.SetIsSeen(false);
	ASSERT_FALSE(mesh.IsSeen());
	ASSERT_FALSE(mesh.GetState().m_isSeen);
}



