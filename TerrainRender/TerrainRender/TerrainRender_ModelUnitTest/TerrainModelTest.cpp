#include "pch.h"
#include "Mocks.h"
#include "TerrainModelSubscriber.h"

class TerrainModelTest : public ::testing::Test {
protected:
	TerrainModelPtr								m_terrainModel;
	std::shared_ptr<TerrainModelSubscriber>		m_subscriber;
	Microsoft::WRL::ComPtr<ID3D11Device>		m_mock_device;
	std::shared_ptr<MockIDataAccess>			m_mock_persistence;
	Microsoft::WRL::ComPtr<ID3D11Device>		m_device;

	void SetUp() override {

		HRESULT hr = D3D11CreateDevice(
			nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &m_device, nullptr, nullptr);
			ASSERT_TRUE(SUCCEEDED(hr)) << "Failed to create D3D11 device";

		m_terrainModel		= std::make_unique<TerrainModel>();
		m_subscriber		= std::make_shared<TerrainModelSubscriber>();
		m_mock_persistence	= std::make_shared<MockIDataAccess>();

		ASSERT_TRUE(m_terrainModel->Initalize(m_mock_persistence, m_device, 800, 600, 1, 500, PI / 3));
		m_terrainModel->m_modelMessageSystem.Subscribe(m_subscriber);
	}
};

TEST_F(TerrainModelTest, Test_InitalizeInvalid)
{
	IDataAccessPtr							perstistence;
	IModelPtr								terrainModel;
	Microsoft::WRL::ComPtr<ID3D11Device>	device;

	terrainModel = std::make_unique<TerrainModel>();

	ASSERT_FALSE(terrainModel->Initalize(perstistence, m_device, 800, 600, 1, 500, PI / 3));
	ASSERT_FALSE(terrainModel->Initalize(m_mock_persistence, device, 800, 600, 1, 500, PI / 3));
	ASSERT_FALSE(terrainModel->Initalize(m_mock_persistence, m_device, 0, 600, 1, 500, PI / 3));
	ASSERT_FALSE(terrainModel->Initalize(m_mock_persistence, m_device, 800, 0, 1, 500, PI / 3));
	ASSERT_FALSE(terrainModel->Initalize(m_mock_persistence, m_device, 800,600, 0, 500, PI / 3));
}

TEST_F(TerrainModelTest, Test_Resize)
{
	IModelPtr								terrainModel;

	terrainModel = std::make_unique<TerrainModel>();

	ASSERT_FALSE(terrainModel->Resize(1024,768));
	ASSERT_FALSE(m_terrainModel->Resize(0, 768));
	ASSERT_TRUE(m_terrainModel->Resize(1024, 768));
}

//	IDM_FLYTHROUGH_NEXT_FRAME,
//	IDM_FLYTHROUGH_START_POSITION,
//	IDM_FLYTHROUGH_STOP,
//	IDM_FLYTHROUGH_SET_FRAME,
//	IDM_FLYTHROUGH_SET_SPEED,

//	IDM_TRAJECTORY_SET_ISSEEN,
//	IDM_TRAJECTORY_ROTATION,
//	IDM_TRAJECTORY_TRANSLATION,
//
//	IDM_MESH_SET_COLOR,
//	IDM_MESH_SET_ISSEEN,
//
//	IDM_CLEAR_TRAJECTORY,
//	IDM_CLEAR_MESHES



TEST_F(TerrainModelTest, Test_HandleMessage_IDM_INVALID)
{
	ASSERT_FALSE(m_terrainModel->HandleMessage(IDM_INVALID, {}, {}, {}));
}
TEST_F(TerrainModelTest, Test_HandleMessage_IDM_LOAD_TERRAIN_SHARP)
{

	std::vector<StlFacet>	empty{};
	EXPECT_CALL(*m_mock_persistence, LoadTerrain_withSharpEdges(testing::StrEq(L"path"))).Times(1);
	EXPECT_CALL(*m_mock_persistence, GetFacets()).Times(1).WillOnce(testing::ReturnRef(empty));;

	ASSERT_FALSE(m_terrainModel->HandleMessage(IDM_LOAD_TERRAIN_SHARP, { L"path" }, {}, {}));

	std::vector<StlFacet>	stlFacets{ StlFacet() };
	EXPECT_CALL(*m_mock_persistence, LoadTerrain_withSharpEdges(testing::StrEq(L"path"))).Times(1);
	EXPECT_CALL(*m_mock_persistence, GetFacets()).Times(1).WillOnce(testing::ReturnRef(stlFacets));;

	ASSERT_TRUE(m_terrainModel->HandleMessage(IDM_LOAD_TERRAIN_SHARP, { L"path" }, {}, {}));
}

TEST_F(TerrainModelTest, Test_HandleMessage_IDM_LOAD_TERRAIN_SOFT)
{
	//IDM_LOAD_TERRAIN_SHARP
	std::vector<StlVertex>		emptyVertices;
	std::vector<CornerIndices>	emptyIndices;
	std::vector<StlVertex>		vertices{ StlVertex()};
	std::vector<CornerIndices>	indices{ CornerIndices()};

	EXPECT_CALL(*m_mock_persistence, LoadTerrain_withSoftEdges(testing::StrEq(L"path"))).Times(1);
	EXPECT_CALL(*m_mock_persistence, GetIndices_Soft()).Times(1).WillOnce(testing::ReturnRef(emptyIndices));
	EXPECT_CALL(*m_mock_persistence, GetVertices_Soft()).Times(1).WillOnce(testing::ReturnRef(emptyVertices));

	ASSERT_FALSE(m_terrainModel->HandleMessage(IDM_LOAD_TERRAIN_SOFT, { L"path" }, {}, {}));


	EXPECT_CALL(*m_mock_persistence, LoadTerrain_withSoftEdges(testing::StrEq(L"path"))).Times(1);
	EXPECT_CALL(*m_mock_persistence, GetIndices_Soft()).Times(1).WillOnce(testing::ReturnRef(indices));
	EXPECT_CALL(*m_mock_persistence, GetVertices_Soft()).Times(1).WillOnce(testing::ReturnRef(vertices));

	ASSERT_TRUE(m_terrainModel->HandleMessage(IDM_LOAD_TERRAIN_SOFT, { L"path" }, {}, {}));
}

TEST_F(TerrainModelTest, Test_HandleMessage_IDM_LOAD_PROJECT_SOFT)
{
	//IDM_LOAD_TERRAIN_SHARP
	std::vector<std::wstring>	filepaths {L"path1", L"path2"};

	EXPECT_CALL(*m_mock_persistence, LoadTerrain_withSoftEdges(testing::StrEq(L"path1"))).Times(1);
	EXPECT_CALL(*m_mock_persistence, LoadTerrain_withSoftEdges(testing::StrEq(L"path2"))).Times(1);

	m_terrainModel->HandleMessage(IDM_LOAD_PROJECT_SOFT, filepaths, {}, {});
}

TEST_F(TerrainModelTest, Test_HandleMessage_IDM_LOAD_PROJECT_SHARP)
{
	//IDM_LOAD_TERRAIN_SHARP
	std::vector<std::wstring>	filepaths{ L"path1", L"path2" };

	EXPECT_CALL(*m_mock_persistence, LoadTerrain_withSharpEdges(testing::StrEq(L"path2"))).Times(1);
	EXPECT_CALL(*m_mock_persistence, LoadTerrain_withSharpEdges(testing::StrEq(L"path1"))).Times(1);

	m_terrainModel->HandleMessage(IDM_LOAD_PROJECT_SHARP, filepaths, {}, {});
}

TEST_F(TerrainModelTest, Test_HandleMessage_IDM_LOAD_CONFIGURATION)
{
	ParameterFile params;

	EXPECT_CALL(*m_mock_persistence, LoadConfigurationFile(testing::StrEq(L"path"), testing::Matcher<ParameterFile&>(testing::_))).Times(1);

	ASSERT_TRUE(m_terrainModel->HandleMessage(IDM_LOAD_CONFIGURATION, { L"path" }, {}, {}));
}

TEST_F(TerrainModelTest, Test_HandleMessage_IDM_LOAD_CAMERA_TRAJECTORY)
{
	std::vector<CameraPose> empty = {};

	EXPECT_CALL(*m_mock_persistence, LoadCameraTrajectory(testing::StrEq(L"path"), empty)).Times(1);
	ASSERT_FALSE(m_terrainModel->HandleMessage(IDM_LOAD_CAMERA_TRAJECTORY, { L"path" }, {}, {}));
}


//	IDM_ORIGO_SET_LONGITUDE,
//	IDM_ORIGO_SET_LATITUDE,
TEST_F(TerrainModelTest, Test_HandleMessage_IDM_ORIGO_SET_LONGITUDE_LATITUDE)
{
	ASSERT_TRUE(m_terrainModel->HandleMessage(IDM_ORIGO_SET_LONGITUDE, {}, { 10.0f }, {}));
	ASSERT_TRUE(m_terrainModel->HandleMessage(IDM_ORIGO_SET_LATITUDE, {}, { 30.0f }, {}));

	ASSERT_FLOAT_EQ(10.f, m_subscriber->m_explore3dState.origo.longitude);
	ASSERT_FLOAT_EQ(30.f, m_subscriber->m_explore3dState.origo.latitude);
}

TEST_F(TerrainModelTest, Test_HandleMessage_IDM_E3D_)
{
	Vector3D startPos				= m_terrainModel->m_camera->GetPositionVec();
	Vector3D previousPos		= m_terrainModel->m_camera->GetPositionVec();
	Vector3D actualPos;

	Vector3D startRot			= m_terrainModel->m_camera->GetRotationVec();
	Vector3D prevRot			= m_terrainModel->m_camera->GetRotationVec();
	Vector3D actualRot;
	
	//InCorrect Parameters

	ASSERT_FALSE(m_terrainModel->HandleMessage(IDM_E3D_MOVE_FORWARD, {}, {}, {}));

	//IDM_E3D_MOVE_FORWARD
	ASSERT_TRUE(m_terrainModel->HandleMessage(IDM_E3D_MOVE_FORWARD, {}, { 1.0f }, {}));

	actualPos = m_terrainModel->m_camera->GetPositionVec();

	ASSERT_FLOAT_EQ(previousPos.x, actualPos.x);
	ASSERT_FLOAT_EQ(previousPos.y, actualPos.y);
	ASSERT_NE(previousPos.z, actualPos.z);

	previousPos = actualPos;

	//IDM_E3D_MOVE_BACK
	ASSERT_TRUE(m_terrainModel->HandleMessage(IDM_E3D_MOVE_BACK, {}, { 1.0f }, {}));

	actualPos = m_terrainModel->m_camera->GetPositionVec();

	ASSERT_FLOAT_EQ(previousPos.x, actualPos.x);
	ASSERT_FLOAT_EQ(previousPos.y, actualPos.y);
	ASSERT_NE(previousPos.z, actualPos.z);

	previousPos = actualPos;

	//IDM_E3D_MOVE_LEFT
	ASSERT_TRUE(m_terrainModel->HandleMessage(IDM_E3D_MOVE_LEFT, {}, { 1.0f }, {}));

	actualPos = m_terrainModel->m_camera->GetPositionVec();

	ASSERT_NE(previousPos.x, actualPos.x);
	ASSERT_FLOAT_EQ(previousPos.y, actualPos.y);
	ASSERT_FLOAT_EQ(previousPos.z, actualPos.z);

	previousPos = actualPos;

	//IDM_E3D_MOVE_RIGHT
	ASSERT_TRUE(m_terrainModel->HandleMessage(IDM_E3D_MOVE_RIGHT, {}, { 1.0f }, {}));

	actualPos = m_terrainModel->m_camera->GetPositionVec();

	ASSERT_NE(previousPos.x, actualPos.x);
	ASSERT_FLOAT_EQ(previousPos.y, actualPos.y);
	ASSERT_FLOAT_EQ(previousPos.z, actualPos.z);

	previousPos = actualPos;

	//IDM_E3D_MOVE_UP
	ASSERT_TRUE(m_terrainModel->HandleMessage(IDM_E3D_MOVE_UP, {}, { 1.0f }, {}));
	actualPos = m_terrainModel->m_camera->GetPositionVec();

	ASSERT_FLOAT_EQ(previousPos.x, actualPos.x);
	ASSERT_NE(previousPos.y, actualPos.y);
	ASSERT_FLOAT_EQ(previousPos.z, actualPos.z);

	previousPos = actualPos;

	//IDM_E3D_MOVE_DOWN
	ASSERT_TRUE(m_terrainModel->HandleMessage(IDM_E3D_MOVE_DOWN, {}, { 1.0f }, {}));

	actualPos = m_terrainModel->m_camera->GetPositionVec();

	ASSERT_FLOAT_EQ(previousPos.x, actualPos.x);
	ASSERT_NE(previousPos.y, actualPos.y);
	ASSERT_FLOAT_EQ(previousPos.z, actualPos.z);

	previousPos = actualPos;

	//IDM_E3D_ROTATE
	ASSERT_TRUE(m_terrainModel->HandleMessage(IDM_E3D_ROTATE, {}, { 1.0f, 2.0f }, {}));

	actualRot = m_terrainModel->m_camera->GetRotationVec();
	ASSERT_NE(prevRot.x, actualRot.x);
	ASSERT_NE(prevRot.y, actualRot.y);
	ASSERT_FLOAT_EQ(prevRot.z, actualRot.z);

	//IDM_E3D_CAMERA_RESET
	ASSERT_TRUE(m_terrainModel->HandleMessage(IDM_E3D_CAMERA_RESET, {}, {}, {}));

	actualPos = m_terrainModel->m_camera->GetPositionVec();
	actualRot = m_terrainModel->m_camera->GetRotationVec();
	ASSERT_EQ(actualPos, startPos);
	ASSERT_EQ(actualRot, startRot);
}

TEST_F(TerrainModelTest, Test_HandleMessage_IDM_SET_CAMERA_)
{
	ASSERT_TRUE(m_terrainModel->HandleMessage(IDM_SET_CAMERA_FIELD_OF_VIEW, {}, {PI/2 }, {}));
	ASSERT_FLOAT_EQ(PI / 2, m_terrainModel->m_camera->GetFOVrad());

	ASSERT_TRUE(m_terrainModel->HandleMessage(IDM_SET_CAMERA_NEAR_SCREEN, {}, { 3.0f }, {}));
	ASSERT_FLOAT_EQ(3.0f, m_terrainModel->m_camera->GetNearScreen());
	m_terrainModel->m_camera->GetFOVrad();

	ASSERT_FALSE(m_terrainModel->HandleMessage(IDM_SET_CAMERA_FAR_SCREEN, {}, { 3.0f }, {}));
	ASSERT_TRUE(m_terrainModel->HandleMessage(IDM_SET_CAMERA_FAR_SCREEN, {}, { 100.0f }, {}));
	ASSERT_FLOAT_EQ(100.0f, m_terrainModel->m_camera->GetFarScreen());
	m_terrainModel->m_camera->GetFOVrad();
}

TEST_F(TerrainModelTest, Test_HandleMessage_IDM_FLYTHROUGH_)
{
	Vector3D startPos = m_terrainModel->m_camera->GetPositionVec();
	Vector3D previousPos = m_terrainModel->m_camera->GetPositionVec();
	Vector3D actualPos;

	Vector3D startRot = m_terrainModel->m_camera->GetRotationVec();
	Vector3D prevRot = m_terrainModel->m_camera->GetRotationVec();
	Vector3D actualRot;

	ASSERT_FALSE(m_terrainModel->HandleMessage(IDM_FLYTHROUGH_START_POSITION, {}, {}, {}));
	ASSERT_FALSE(m_terrainModel->HandleMessage(IDM_FLYTHROUGH_NEXT_FRAME, {}, {}, {}));
	ASSERT_FALSE(m_terrainModel->HandleMessage(IDM_FLYTHROUGH_STOP, {}, {}, {}));
	ASSERT_FALSE(m_terrainModel->HandleMessage(IDM_FLYTHROUGH_SET_FRAME, {}, {}, {}));
	ASSERT_FALSE(m_terrainModel->HandleMessage(IDM_FLYTHROUGH_SET_SPEED, {}, {}, {}));
}

//	IDM_SET_TIME_E3D,
//	IDM_SET_START_TIME_TRAJECTORY,

TEST_F(TerrainModelTest, Test_HandleMessage_IDM_SET_TIME)
{
	ASSERT_FALSE(m_terrainModel->HandleMessage(IDM_SET_TIME_E3D, {}, {}, {}));
	ASSERT_TRUE(m_terrainModel->HandleMessage(IDM_SET_TIME_E3D, {}, {}, {100}));

	ASSERT_EQ(100, m_subscriber->m_explore3dState.currentEpochTime.getSeconds());

	//Camera Trajectory is not init
	ASSERT_FALSE(m_terrainModel->HandleMessage(IDM_SET_START_TIME_TRAJECTORY, {}, {}, {}));
	ASSERT_TRUE(m_terrainModel->HandleMessage(IDM_SET_START_TIME_TRAJECTORY, {}, {}, { 1000 }));

	ASSERT_EQ(0, m_subscriber->m_flythroughState.currentEpochTime.getSeconds());
}

TEST_F(TerrainModelTest, Test_HandleMessage_IDM_MESH_GROUP)
{
	Vector3D expectedScale			= { 2.0f, 2.0f, 2.0f };
	Vector3D expectedRotation		= { PI/2, PI/3, PI/4 };
	Vector3D expectedTranslation	= { 100.0f, 0.0f, -200.0f };

	ASSERT_FALSE(m_terrainModel->HandleMessage(IDM_MESH_GROUP_SCALE, {}, {}, {}));
	ASSERT_TRUE(m_terrainModel->HandleMessage(IDM_MESH_GROUP_SCALE, {}, { expectedScale.x }, {}));

	ASSERT_EQ(expectedScale, m_subscriber->m_TerrainsState.scale);

	ASSERT_FALSE(m_terrainModel->HandleMessage(IDM_MESH_GROUP_ROTATION, {}, {}, {}));
	ASSERT_TRUE(m_terrainModel->HandleMessage(IDM_MESH_GROUP_ROTATION, {}, { expectedRotation.x, expectedRotation.y, expectedRotation.z}, {}));

	ASSERT_EQ(expectedRotation, m_subscriber->m_TerrainsState.rotation);

	ASSERT_FALSE(m_terrainModel->HandleMessage(IDM_MESH_GROUP_TRANSLATION, {}, {}, {}));
	ASSERT_TRUE(m_terrainModel->HandleMessage(IDM_MESH_GROUP_TRANSLATION, {}, { expectedTranslation.x, expectedTranslation.y, expectedTranslation.z }, {}));

	ASSERT_EQ(expectedTranslation, m_subscriber->m_TerrainsState.translation);
}


