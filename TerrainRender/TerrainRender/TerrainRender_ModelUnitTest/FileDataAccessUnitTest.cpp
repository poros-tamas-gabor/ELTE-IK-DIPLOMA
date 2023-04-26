#include "pch.h"

class DataAccessTest : public ::testing::Test {
protected:
    IDataAccessPtr m_persistence;

    void SetUp() override {
        m_persistence = std::make_shared<BinaryFileDataAccessAsync>();
    }
};

TEST_F(DataAccessTest, LoadConfigurationFile_InvalidPath_ThrowsTRException) {
    ParameterFile file;

    ASSERT_THROW(m_persistence->LoadConfigurationFile(L"invalid", file), TRException);
}

TEST_F(DataAccessTest, LoadConfigurationFile_IncorrectHeader_ThrowsTRException) {
    ParameterFile file;

    TCHAR buffer[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, buffer);

    std::wstring incorrectHeader_path(buffer);
    incorrectHeader_path += L"..\\..\\..\\TerrainRender_ModelUnitTest\\ResourceFiles\\incorrect_header.json";

    ASSERT_THROW(m_persistence->LoadConfigurationFile(incorrectHeader_path.c_str(), file), TRException);
}

TEST_F(DataAccessTest, LoadConfigurationFile_CorrectFile_LoadsSuccessfully) {
    ParameterFile file;

    TCHAR buffer[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, buffer);

    std::wstring correctFile_path(buffer);
    correctFile_path += L"..\\..\\..\\TerrainRender_ModelUnitTest\\ResourceFiles\\testConfig.json";
    m_persistence->LoadConfigurationFile(correctFile_path.c_str(), file);

    ASSERT_DOUBLE_EQ(file.origo.latitude, 47.497913);
    ASSERT_DOUBLE_EQ(file.origo.longitude, 19.040236);
    ASSERT_EQ(file.terrain.translation, Vector3D(10, 11, 12));
    ASSERT_EQ(file.terrain.rotation, Vector3D(20, 21, 22));
    ASSERT_FALSE(file.terrain.colors.empty());
    ASSERT_EQ(file.terrain.colors.size(), size_t(3));
    ASSERT_EQ(file.trajectory.translation, Vector3D(700, 65, -250));
    ASSERT_EQ(file.trajectory.rotation, Vector3D(0, 0, 0));
}

TEST_F(DataAccessTest, LoadCameraTrajectory0)
{
	std::vector<CameraPose> cameraPoses;
	ASSERT_THROW(m_persistence->LoadCameraTrajectory(L"invalid", cameraPoses), TRException);
}

TEST_F(DataAccessTest, LoadCameraTrajectory1)
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

TEST_F(DataAccessTest, LoadTerrain_withSoftEdges)
{
	IDataAccessPtr persistence = std::make_shared<BinaryFileDataAccessAsync>();
	ASSERT_THROW(persistence->LoadTerrain_withSoftEdges(L"invalid"), TRException);

	TCHAR buffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, buffer);

	std::wstring asciiPath(buffer);
	asciiPath += L"..\\..\\..\\TerrainRender_ModelUnitTest\\ResourceFiles\\ASCII_Sphericon.stl";
	ASSERT_THROW(persistence->LoadTerrain_withSoftEdges(asciiPath.c_str()), TRException);


	std::wstring spherePath(buffer);
	spherePath += L"..\\..\\..\\TerrainRender_ModelUnitTest\\ResourceFiles\\3d_model_of_Sphere.stl";
	persistence->LoadTerrain_withSoftEdges(spherePath.c_str());

	ASSERT_EQ(persistence->GetVertices_Soft().size(), size_t(1538));
	ASSERT_EQ(persistence->GetIndices_Soft().size(), size_t(3072));

	std::wstring bunny70k_path(buffer);
	bunny70k_path += L"..\\..\\..\\TerrainRender_ModelUnitTest\\ResourceFiles\\bunny70k.stl";
	persistence->LoadTerrain_withSoftEdges(bunny70k_path.c_str());

	ASSERT_EQ(persistence->GetVertices_Soft().size(), size_t(34834));
	ASSERT_EQ(persistence->GetIndices_Soft().size(), size_t(69451));

	std::wstring bunny10k_path(buffer);
	bunny10k_path += L"..\\..\\..\\TerrainRender_ModelUnitTest\\ResourceFiles\\bunny10k.stl";
	persistence->LoadTerrain_withSoftEdges(bunny10k_path.c_str());

	ASSERT_EQ(persistence->GetVertices_Soft().size(), size_t(5051));
	ASSERT_EQ(persistence->GetIndices_Soft().size(), size_t(9999));
}


