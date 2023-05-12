#include "pch.h"

class DataAccessTest : public ::testing::Test {
protected:
    IDataAccessPtr m_persistence;

    void SetUp() override {
        m_persistence = std::make_shared<BinaryFileDataAccessAsync>();
    }
};

TEST_F(DataAccessTest, LoadConfigurationFile_InvalidPath) {
    ParameterFile file;

    ASSERT_THROW(m_persistence->LoadConfigurationFile(L"invalid", file), TRException);
}

TEST_F(DataAccessTest, LoadConfigurationFile_Incorrect) {
    ParameterFile file;

    TCHAR buffer[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, buffer);

    std::wstring incorrectHeader_path(buffer);
    incorrectHeader_path += L"..\\..\\..\\TerrainRender_ModelUnitTest\\ResourceFiles\\incorrect.json";

	ASSERT_ANY_THROW(m_persistence->LoadConfigurationFile(incorrectHeader_path.c_str(), file) );
}

TEST_F(DataAccessTest, LoadConfigurationFile) {
    ParameterFile file;

    TCHAR buffer[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, buffer);

    std::wstring correctFile_path(buffer);
    correctFile_path += L"..\\..\\..\\TerrainRender_ModelUnitTest\\ResourceFiles\\testConfig.json";
    m_persistence->LoadConfigurationFile(correctFile_path.c_str(), file);

    ASSERT_FLOAT_EQ(file.origo.latitude, 47.497913f);
	ASSERT_FLOAT_EQ(file.origo.longitude, 19.040236f);
	ASSERT_FLOAT_EQ(file.camera.fieldOfView, 1.0467f);
	ASSERT_FLOAT_EQ(file.camera.screenNear , 1.0f);
	ASSERT_FLOAT_EQ(file.camera.screenDepth, 2000.0f);
    ASSERT_EQ(file.terrain.translation, Vector3D(10, 11, 12));
    ASSERT_EQ(file.terrain.rotation, Vector3D(20, 21, 22));
	ASSERT_EQ(file.terrain.scale, Vector3D(1, 2, 3));
    ASSERT_FALSE(file.terrain.colors.empty());
    ASSERT_EQ(file.terrain.colors.size(), size_t(3));
    ASSERT_EQ(file.trajectory.translation, Vector3D(700, 65, -250));
    ASSERT_EQ(file.trajectory.rotation, Vector3D(0, 0, 0));
}

TEST_F(DataAccessTest, LoadCameraTrajectory_InvalidPath)
{
	std::vector<CameraPose> cameraPoses;
	ASSERT_THROW(m_persistence->LoadCameraTrajectory(L"invalid", cameraPoses), TRException);
}

TEST_F(DataAccessTest, LoadCameraTrajectory_Incorrect)
{
	std::vector<CameraPose> cameraPoses;
	ASSERT_THROW(m_persistence->LoadCameraTrajectory(L"..\\..\\..\\TerrainRender_ModelUnitTest\\ResourceFiles\\incorrect.csv", cameraPoses), TRException);
}
TEST_F(DataAccessTest, LoadCameraTrajectory)
{
	CameraPose		test = { {1664534691, 679506806}, 1.37945, -0.0253926,0.00191395 , 188.429, -675.288, 0.000213623 };
	std::vector<CameraPose> cameraPoses;
	TCHAR buffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, buffer);


	std::wstring path(buffer);
	path += L"..\\..\\..\\TerrainRender_ModelUnitTest\\ResourceFiles\\FR_Video_409.csv";
	m_persistence->LoadCameraTrajectory(path.c_str(), cameraPoses);

	ASSERT_EQ(cameraPoses.size(), size_t(1670));
	ASSERT_EQ(cameraPoses.at(9), test);
}

TEST_F(DataAccessTest, LoadTerrain_withSharpEdges_InvalidPath)
{
	ASSERT_THROW(m_persistence->LoadTerrain_withSharpEdges(L"invalid"), TRException);
}

TEST_F(DataAccessTest, LoadTerrain_withSharpEdges_Incorrect)
{
	TCHAR buffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, buffer);

	std::wstring asciiPath(buffer);

	asciiPath += L"..\\..\\..\\TerrainRender_ModelUnitTest\\ResourceFiles\\ASCII_Sphericon.stl";
	ASSERT_THROW(m_persistence->LoadTerrain_withSharpEdges(asciiPath.c_str()), TRException);
}
TEST_F(DataAccessTest, LoadTerrain_withSharpEdges)
{
	TCHAR buffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, buffer);


	std::wstring spherePath(buffer);
	spherePath += L"..\\..\\..\\TerrainRender_ModelUnitTest\\ResourceFiles\\3d_model_of_Sphere.stl";
	m_persistence->LoadTerrain_withSharpEdges(spherePath.c_str());

	ASSERT_EQ(m_persistence->GetFacets_Sharp().size(), size_t(3072));

	std::wstring bunny70k_path(buffer);
	bunny70k_path += L"..\\..\\..\\TerrainRender_ModelUnitTest\\ResourceFiles\\bunny70k.stl";
	m_persistence->LoadTerrain_withSharpEdges(bunny70k_path.c_str());

	ASSERT_EQ(m_persistence->GetFacets_Sharp().size(), size_t(69451));

	std::wstring bunny10k_path(buffer);
	bunny10k_path += L"..\\..\\..\\TerrainRender_ModelUnitTest\\ResourceFiles\\bunny10k.stl";
	m_persistence->LoadTerrain_withSharpEdges(bunny10k_path.c_str());

	ASSERT_EQ(m_persistence->GetFacets_Sharp().size(), size_t(9999));
}

TEST_F(DataAccessTest, LoadTerrain_withSoftEdges_InvalidPath)
{
	ASSERT_THROW(m_persistence->LoadTerrain_withSoftEdges(L"invalid"), TRException);
}

TEST_F(DataAccessTest, LoadTerrain_withSoftEdges_Incorrect)
{
	TCHAR buffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, buffer);

	std::wstring asciiPath(buffer);

	asciiPath += L"..\\..\\..\\TerrainRender_ModelUnitTest\\ResourceFiles\\ASCII_Sphericon.stl";
	ASSERT_THROW(m_persistence->LoadTerrain_withSoftEdges(asciiPath.c_str()), TRException);
}
TEST_F(DataAccessTest, LoadTerrain_withSoftEdges)
{
	TCHAR buffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, buffer);


	std::wstring spherePath(buffer);
	spherePath += L"..\\..\\..\\TerrainRender_ModelUnitTest\\ResourceFiles\\3d_model_of_Sphere.stl";
	m_persistence->LoadTerrain_withSoftEdges(spherePath.c_str());

	ASSERT_EQ(m_persistence->GetVertices_Soft().size(), size_t(1538));
	ASSERT_EQ(m_persistence->GetIndices_Soft().size(), size_t(3072));

	std::wstring bunny70k_path(buffer);
	bunny70k_path += L"..\\..\\..\\TerrainRender_ModelUnitTest\\ResourceFiles\\bunny70k.stl";
	m_persistence->LoadTerrain_withSoftEdges(bunny70k_path.c_str());

	ASSERT_EQ(m_persistence->GetVertices_Soft().size(), size_t(34834));
	ASSERT_EQ(m_persistence->GetIndices_Soft().size(), size_t(69451));

	std::wstring bunny10k_path(buffer);
	bunny10k_path += L"..\\..\\..\\TerrainRender_ModelUnitTest\\ResourceFiles\\bunny10k.stl";
	m_persistence->LoadTerrain_withSoftEdges(bunny10k_path.c_str());

	ASSERT_EQ(m_persistence->GetVertices_Soft().size(), size_t(5051));
	ASSERT_EQ(m_persistence->GetIndices_Soft().size(), size_t(9999));
}


