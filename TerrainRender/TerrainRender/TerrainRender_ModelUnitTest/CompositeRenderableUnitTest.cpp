#include "Mocks.h"
#include "pch.h"

using ::testing::Return;

class CompositeRenderableUnitTest : public ::testing::Test
{
protected:
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	IVertexShaderPtr m_vertexShader;
	IPixelShaderPtr m_pixelShader;

	//unsigned _vertexCount = 4;
	//unsigned _indexCount = 6;
	//VertexMesh m_vertices[4] = { VertexMesh(),VertexMesh(),VertexMesh(),VertexMesh() };
	//unsigned long m_indices[6] = { 0,1,2,2,3,4 };

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

TEST_F(CompositeRenderableUnitTest, InitializeReturnsTrueWithValidParameters)
{
	CompositeRenderable<VertexMesh> meshes;
	ASSERT_TRUE(meshes.Initialize(m_device, m_vertexShader, m_pixelShader, NULL, NULL, NULL, NULL));
}

TEST_F(CompositeRenderableUnitTest, InitializeReturnsFalseWithNullVertexShader)
{

	CompositeRenderable<VertexMesh> meshes;
	bool result = meshes.Initialize(m_device, nullptr, m_pixelShader, NULL, NULL, NULL, NULL);
	EXPECT_FALSE(result);
}
TEST_F(CompositeRenderableUnitTest, InitializeReturnsFalseWithNullDevice)
{
	CompositeRenderable<VertexMesh> meshes;
	bool result = meshes.Initialize(nullptr, m_vertexShader, m_pixelShader, NULL, NULL, NULL, NULL);
	EXPECT_FALSE(result);
}
TEST_F(CompositeRenderableUnitTest, InitializeReturnsFalseWithNullPixelShader)
{
	CompositeRenderable<VertexMesh> meshes;
	bool result = meshes.Initialize(m_device, m_vertexShader, nullptr, NULL, NULL, NULL, NULL);
	EXPECT_FALSE(result);
}
TEST_F(CompositeRenderableUnitTest, Add)
{
	CompositeRenderable<VertexMesh> meshes;

	unsigned vertexCount = 4;
	unsigned indexCount = 6;
	VertexMesh vertices[4] = { VertexMesh(),VertexMesh(),VertexMesh(),VertexMesh() };
	unsigned long indices[6] = { 0,1,2,2,3,4 };

	ASSERT_TRUE(meshes.Initialize(m_device, m_vertexShader, m_pixelShader, NULL, NULL, NULL, NULL));
	ASSERT_TRUE(meshes.Add(vertices, indices, vertexCount, indexCount, PolygonMeshCreator(), L"test"));
}

TEST_F(CompositeRenderableUnitTest, Remove)
{
	CompositeRenderable<VertexMesh> meshes;

	unsigned vertexCount = 4;
	unsigned indexCount = 6;
	VertexMesh vertices[4] = { VertexMesh(),VertexMesh(),VertexMesh(),VertexMesh() };
	unsigned long indices[6] = { 0,1,2,2,3,4 };

	ASSERT_TRUE(meshes.Initialize(m_device, m_vertexShader, m_pixelShader, NULL, NULL, NULL, NULL));

	ASSERT_THROW(meshes.GetLastAddedComponent(), TRException);
	ASSERT_TRUE(meshes.Add(vertices, indices, vertexCount, indexCount, PolygonMeshCreator(), L"test"));
	IRendarablePtr<VertexMesh> last = meshes.GetLastAddedComponent();

	ASSERT_TRUE(meshes.Remove(last));
	ASSERT_FALSE(meshes.Remove(last));
}

TEST_F(CompositeRenderableUnitTest, ClearRenderable)
{
	CompositeRenderable<VertexMesh> meshes;

	unsigned vertexCount = 4;
	unsigned indexCount = 6;
	VertexMesh vertices[4] = { VertexMesh(),VertexMesh(),VertexMesh(),VertexMesh() };
	unsigned long indices[6] = { 0,1,2,2,3,4 };

	ASSERT_TRUE(meshes.Initialize(m_device, m_vertexShader, m_pixelShader, NULL, NULL, NULL, NULL));

	ASSERT_TRUE(meshes.Add(vertices, indices, vertexCount, indexCount, PolygonMeshCreator(), L"test"));
	IRendarablePtr<VertexMesh> last = meshes.GetLastAddedComponent();

	meshes.ClearRenderables();

	ASSERT_FALSE(meshes.Remove(last));
}

TEST_F(CompositeRenderableUnitTest, Rotate)
{
	CompositeRenderable<VertexMesh> meshes;

	unsigned vertexCount = 4;
	unsigned indexCount = 6;
	VertexMesh vertices[4] = { VertexMesh(),VertexMesh(),VertexMesh(),VertexMesh() };
	unsigned long indices[6] = { 0,1,2,2,3,4 };

	ASSERT_TRUE(meshes.Initialize(m_device, m_vertexShader, m_pixelShader, NULL, NULL, NULL, NULL));

	ASSERT_TRUE(meshes.Add(vertices, indices, vertexCount, indexCount, PolygonMeshCreator(), L"test"));
	IRendarablePtr<VertexMesh> fst = meshes.GetLastAddedComponent();

	ASSERT_TRUE(meshes.Add(vertices, indices, vertexCount, indexCount, PolygonMeshCreator(), L"test"));
	IRendarablePtr<VertexMesh> snd = meshes.GetLastAddedComponent();

	meshes.Rotate(1.0f, -1.0f, 1.0f);

	ASSERT_EQ(fst->GetState().rotation, Vector3D(1.0f, -1.0f, 1.0f));
	ASSERT_EQ(snd->GetState().rotation, Vector3D(1.0f, -1.0f, 1.0f));
}

TEST_F(CompositeRenderableUnitTest, RotateComponent)
{
	CompositeRenderable<VertexMesh> meshes;

	unsigned vertexCount = 4;
	unsigned indexCount = 6;
	VertexMesh vertices[4] = { VertexMesh(),VertexMesh(),VertexMesh(),VertexMesh() };
	unsigned long indices[6] = { 0,1,2,2,3,4 };

	ASSERT_TRUE(meshes.Initialize(m_device, m_vertexShader, m_pixelShader, NULL, NULL, NULL, NULL));

	ASSERT_TRUE(meshes.Add(vertices, indices, vertexCount, indexCount, PolygonMeshCreator(), L"test"));
	IRendarablePtr<VertexMesh> fst = meshes.GetLastAddedComponent();

	ASSERT_TRUE(meshes.Add(vertices, indices, vertexCount, indexCount, PolygonMeshCreator(), L"test"));
	IRendarablePtr<VertexMesh> snd = meshes.GetLastAddedComponent();

	meshes.RotateComponent(fst->GetID(), 1.0f, -1.0f, 1.0f);

	ASSERT_EQ(fst->GetState().rotation, Vector3D(1.0f, -1.0f, 1.0f));
	ASSERT_EQ(snd->GetState().rotation, Vector3D(0.0f, 0.0f, 0.0f));
}

TEST_F(CompositeRenderableUnitTest, Translate)
{
	CompositeRenderable<VertexMesh> meshes;

	unsigned vertexCount = 4;
	unsigned indexCount = 6;
	VertexMesh vertices[4] = { VertexMesh(),VertexMesh(),VertexMesh(),VertexMesh() };
	unsigned long indices[6] = { 0,1,2,2,3,4 };

	ASSERT_TRUE(meshes.Initialize(m_device, m_vertexShader, m_pixelShader, NULL, NULL, NULL, NULL));

	ASSERT_TRUE(meshes.Add(vertices, indices, vertexCount, indexCount, PolygonMeshCreator(), L"test"));
	IRendarablePtr<VertexMesh> fst = meshes.GetLastAddedComponent();

	ASSERT_TRUE(meshes.Add(vertices, indices, vertexCount, indexCount, PolygonMeshCreator(), L"test"));
	IRendarablePtr<VertexMesh> snd = meshes.GetLastAddedComponent();

	meshes.Translate(1.0f, -1.0f, 1.0f);

	ASSERT_EQ(fst->GetState().translation, Vector3D(1.0f, -1.0f, 1.0f));
	ASSERT_EQ(snd->GetState().translation, Vector3D(1.0f, -1.0f, 1.0f));
}

TEST_F(CompositeRenderableUnitTest, TranslateComponent)
{
	CompositeRenderable<VertexMesh> meshes;

	unsigned vertexCount = 4;
	unsigned indexCount = 6;
	VertexMesh vertices[4] = { VertexMesh(),VertexMesh(),VertexMesh(),VertexMesh() };
	unsigned long indices[6] = { 0,1,2,2,3,4 };

	ASSERT_TRUE(meshes.Initialize(m_device, m_vertexShader, m_pixelShader, NULL, NULL, NULL, NULL));

	ASSERT_TRUE(meshes.Add(vertices, indices, vertexCount, indexCount, PolygonMeshCreator(), L"test"));
	IRendarablePtr<VertexMesh> fst = meshes.GetLastAddedComponent();

	ASSERT_TRUE(meshes.Add(vertices, indices, vertexCount, indexCount, PolygonMeshCreator(), L"test"));
	IRendarablePtr<VertexMesh> snd = meshes.GetLastAddedComponent();

	meshes.TranslateComponent(fst->GetID(), 1.0f, -1.0f, 1.0f);

	ASSERT_EQ(fst->GetState().translation, Vector3D(1.0f, -1.0f, 1.0f));
	ASSERT_EQ(snd->GetState().translation, Vector3D(0.0f, 0.0f, 0.0f));
}

TEST_F(CompositeRenderableUnitTest, Scale)
{
	CompositeRenderable<VertexMesh> meshes;

	unsigned vertexCount = 4;
	unsigned indexCount = 6;
	VertexMesh vertices[4] = { VertexMesh(),VertexMesh(),VertexMesh(),VertexMesh() };
	unsigned long indices[6] = { 0,1,2,2,3,4 };

	ASSERT_TRUE(meshes.Initialize(m_device, m_vertexShader, m_pixelShader, NULL, NULL, NULL, NULL));

	ASSERT_TRUE(meshes.Add(vertices, indices, vertexCount, indexCount, PolygonMeshCreator(), L"test"));
	IRendarablePtr<VertexMesh> fst = meshes.GetLastAddedComponent();

	ASSERT_TRUE(meshes.Add(vertices, indices, vertexCount, indexCount, PolygonMeshCreator(), L"test"));
	IRendarablePtr<VertexMesh> snd = meshes.GetLastAddedComponent();

	meshes.Scale(1.0f, -1.0f, 1.0f);

	ASSERT_EQ(fst->GetState().scale, Vector3D(1.0f, -1.0f, 1.0f));
	ASSERT_EQ(snd->GetState().scale, Vector3D(1.0f, -1.0f, 1.0f));
}

TEST_F(CompositeRenderableUnitTest, ScaleComponent)
{
	CompositeRenderable<VertexMesh> meshes;

	unsigned vertexCount = 4;
	unsigned indexCount = 6;
	VertexMesh vertices[4] = { VertexMesh(),VertexMesh(),VertexMesh(),VertexMesh() };
	unsigned long indices[6] = { 0,1,2,2,3,4 };

	ASSERT_TRUE(meshes.Initialize(m_device, m_vertexShader, m_pixelShader, NULL, NULL, NULL, NULL));

	ASSERT_TRUE(meshes.Add(vertices, indices, vertexCount, indexCount, PolygonMeshCreator(), L"test"));
	IRendarablePtr<VertexMesh> fst = meshes.GetLastAddedComponent();

	ASSERT_TRUE(meshes.Add(vertices, indices, vertexCount, indexCount, PolygonMeshCreator(), L"test"));
	IRendarablePtr<VertexMesh> snd = meshes.GetLastAddedComponent();

	meshes.ScaleComponent(fst->GetID(), 1.0f, -1.0f, 1.0f);

	ASSERT_EQ(fst->GetState().scale, Vector3D(1.0f, -1.0f, 1.0f));
	ASSERT_EQ(snd->GetState().scale, Vector3D(1.0f, 1.0f, 1.0f));
}

TEST_F(CompositeRenderableUnitTest, SetColor)
{
	CompositeRenderable<VertexMesh> meshes;

	unsigned vertexCount = 4;
	unsigned indexCount = 6;
	VertexMesh vertices[4] = { VertexMesh(),VertexMesh(),VertexMesh(),VertexMesh() };
	unsigned long indices[6] = { 0,1,2,2,3,4 };

	ASSERT_TRUE(meshes.Initialize(m_device, m_vertexShader, m_pixelShader, NULL, NULL, NULL, NULL));

	ASSERT_TRUE(meshes.Add(vertices, indices, vertexCount, indexCount, PolygonMeshCreator(), L"test"));
	IRendarablePtr<VertexMesh> fst = meshes.GetLastAddedComponent();

	ASSERT_TRUE(meshes.Add(vertices, indices, vertexCount, indexCount, PolygonMeshCreator(), L"test"));
	IRendarablePtr<VertexMesh> snd = meshes.GetLastAddedComponent();

	meshes.SetColor(0.5f, 0.5f, 0.5f, 1.0f);

	ASSERT_EQ(fst->GetState().color, Vector4D(0.5f, 0.5f, 0.5f, 1.0f));
	ASSERT_EQ(snd->GetState().color, Vector4D(0.5f, 0.5f, 0.5f, 1.0f));
}

TEST_F(CompositeRenderableUnitTest, SetColorComponent)
{
	CompositeRenderable<VertexMesh> meshes;

	unsigned vertexCount = 4;
	unsigned indexCount = 6;
	VertexMesh vertices[4] = { VertexMesh(),VertexMesh(),VertexMesh(),VertexMesh() };
	unsigned long indices[6] = { 0,1,2,2,3,4 };

	ASSERT_TRUE(meshes.Initialize(m_device, m_vertexShader, m_pixelShader, NULL, NULL, NULL, NULL));

	ASSERT_TRUE(meshes.Add(vertices, indices, vertexCount, indexCount, PolygonMeshCreator(), L"test"));
	IRendarablePtr<VertexMesh> fst = meshes.GetLastAddedComponent();

	ASSERT_TRUE(meshes.Add(vertices, indices, vertexCount, indexCount, PolygonMeshCreator(), L"test"));
	IRendarablePtr<VertexMesh> snd = meshes.GetLastAddedComponent();

	meshes.SetColorComponent(fst->GetID(), 0.5f, 0.5f, 0.5f, 1.0f);

	ASSERT_EQ(fst->GetState().color, Vector4D(0.5f, 0.5f, 0.5f, 1.0f));
	ASSERT_EQ(snd->GetState().color, Vector4D(1.0f, 1.0f, 1.0f, 1.0f));
}