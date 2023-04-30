#include "pch.h"
#include "Mocks.h"

class TerrainModelTest : public ::testing::Test {
protected:
private:

	IModelPtr									m_terrainModel;
	IVertexShaderPtr							m_mock_vertexShaderMesh;
	PixelShaderMeshPtr							m_mock_pixelShaderMesh;
	IVertexShaderPtr							m_mock_vertexShaderPolyLine;
	IPixelShaderPtr								m_mock_pixelShaderPolyLine;

	CompositeRenderable<VertexMesh>				m_mock_meshes;
	CompositeRenderable<VertexPolyLine>			m_mock_polylines;
	MockLight									m_mock_light;
	MockCameraPositioner						m_mock_cameraPositioner;
	MockCameraTrajectory						m_mock_cameraTrajectory;

	Microsoft::WRL::ComPtr<ID3D11Device>		m_mock_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>	m_mock_deviceContext;
	IDataAccessPtr								m_mock_persistence;

	MockCamera									m_mock_camera;
	MockModelMessageSystem						m_mock_modelMessageSystem;

    void SetUp() override {
		//m_mock_vertexShaderMesh = std::make_shared<MockIVertexShader>();
		//m_mock_vertexShaderPolyLine = std::make_shared<MockIVertexShader>();
		//m_mock_pixelShaderPolyLine = std::make_shared<MockIPixelShader>();
		//m_mock_pixelShaderMesh = std::make_shared<MockPixelShaderMesh>();
		//m_mock_persistence = std::make_shared<MockIDataAccess>();
		//m_terrainModel = std::make_shared<TerrainModel>();
    }

};
