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

TEST_F(PolygonMeshUnitTest, InitializeReturnsTrueWithValidParameters)
{
	PolygonMesh mesh;
	ASSERT_TRUE(mesh.Initialize(m_device, m_vertexShader, m_pixelShader, m_vertices, m_indices, _vertexCount, _indexCount));
}
TEST_F(PolygonMeshUnitTest, InitializeReturnsFalseWithNullVertexShader)
{

	PolygonMesh mesh;
	bool result = mesh.Initialize(m_device, nullptr, m_pixelShader, m_vertices, m_indices, _vertexCount, _indexCount);
	EXPECT_FALSE(result);
}

// Test case to check if the Initialize method returns false when the device parameter is null
TEST_F(PolygonMeshUnitTest, InitializeReturnsFalseWithNullDevice)
{
	PolygonMesh mesh;
	bool result = mesh.Initialize(nullptr, m_vertexShader, m_pixelShader, m_vertices, m_indices, _vertexCount, _indexCount);
	EXPECT_FALSE(result);
}

// Test case to check if the Initialize method returns false when the vertex shader parameter is null
TEST_F(PolygonMeshUnitTest, InitializeReturnsFalseWithNullPixelShader)
{
	PolygonMesh mesh;

	bool result = mesh.Initialize(m_device, m_vertexShader, nullptr, m_vertices, m_indices, _vertexCount, _indexCount);

	EXPECT_FALSE(result);
}

TEST_F(PolygonMeshUnitTest, SetNameAndGetName)
{
	PolygonMesh mesh;
	ASSERT_TRUE(mesh.Initialize(m_device, m_vertexShader, m_pixelShader, m_vertices, m_indices, _vertexCount, _indexCount));
	mesh.SetName(L"TestMesh");
	ASSERT_EQ(mesh.GetName(), L"TestMesh");
	ASSERT_EQ(mesh.GetState().name, L"TestMesh");

}
//
//TEST_F(PolygonMeshUnitTest, TransformationFunctions)
//{
//	PolygonMesh mesh;
//	ASSERT_TRUE(mesh.Initialize(m_device, m_vertexShader, m_pixelShader, m_vertices, m_indices, _vertexCount, _indexCount));
//
//	DirectX::XMFLOAT4 actual = mesh.GetState();
//		
//	mesh.Rotate(1.0f, 2.0f, 3.0f);
//	mesh.Translate(4.0f, 5.0f, 6.0f);
//	mesh.Scale(2.0f, 2.0f, 2.0f);
//	mesh.ResetTransformation();
//	//DirectX::XMMATRIX expected = DirectX::XMMatrixIdentity();
//	//DirectX::XMMATRIX actual = mesh.GetWorldMatrix();
//	//ASSERT_TRUE(DirectX::XMMatrixNearEqual(expected, actual, 0.00001f));
//}
TEST_F(PolygonMeshUnitTest, SetColor)
{
	PolygonMesh mesh;
	mesh.SetColor(1.0f, 0.5f, 0.0f, 1.0f);

	Vector4D	expected = { 1.0f, 0.5f, 0.0f, 1.0f };
	Vector4D	actual = mesh.GetState().color;
	ASSERT_EQ(expected, actual);
}

TEST_F(PolygonMeshUnitTest, SetIsSeenAndIsSeen)
{
	PolygonMesh mesh;
	mesh.SetIsSeen(true);
	ASSERT_TRUE(mesh.IsSeen());
	ASSERT_TRUE(mesh.GetState().m_isSeen);
	mesh.SetIsSeen(false);
	ASSERT_FALSE(mesh.IsSeen());
	ASSERT_FALSE(mesh.GetState().m_isSeen);
}

