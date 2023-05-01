#include "Mocks.h"

class ControllerGuiTest : public ::testing::Test {
protected:
	std::shared_ptr<MockIModel>						m_mock_model;
	std::shared_ptr<MockIView>						m_mock_view;
	std::shared_ptr<MockKeyboard>					m_mock_keyboard;
	std::shared_ptr<MockMouse>						m_mock_mouse;
	GuiController									m_controller;

	void SetUp() override {

		m_mock_model			= std::make_shared<MockIModel>();
		m_mock_view				= std::make_shared<MockIView>();
		m_mock_keyboard			= std::make_shared<MockKeyboard>();
		m_mock_mouse			= std::make_shared<MockMouse>();

		ASSERT_TRUE((m_controller.Initialize(m_mock_model, m_mock_view, m_mock_mouse, m_mock_keyboard)));
	}
};

TEST_F(ControllerGuiTest, Initialize)
{
	IModelPtr									mock_model;
	IViewPtr									mock_view;
	KeyboardPtr									mock_keyboard;
	MousePtr									mock_mouse;
	ControllerFlythrough						controller;


	ASSERT_FALSE((controller.Initialize(mock_model, m_mock_view, m_mock_mouse, m_mock_keyboard)));
	ASSERT_FALSE((controller.Initialize(m_mock_model, mock_view, m_mock_mouse, m_mock_keyboard)));
	ASSERT_FALSE((controller.Initialize(m_mock_model, m_mock_view, mock_mouse, m_mock_keyboard)));
	ASSERT_FALSE((controller.Initialize(m_mock_model, m_mock_view, m_mock_mouse, mock_keyboard)));

	ASSERT_TRUE((controller.Initialize(m_mock_model, m_mock_view, m_mock_mouse, m_mock_keyboard)));
	ASSERT_FALSE((controller.IsActive()));
}

//Fake functions

void OpenFileDialog(std::wstring& filePath, const wchar_t* filter)
{
	filePath = L"filepath";
}
void OpenFileDialogDirectory(std::wstring& directory)
{
	directory = L"directoryPath";
}
void OpenFileDialogMultipleSelection(std::vector<std::wstring>& files, const wchar_t* filter)
{
	files = { L"path1", L"path2" };
}
TEST_F(ControllerGuiTest, HandleMessage)
{
	ASSERT_TRUE(m_controller.HandleMessage(IDCC_IS_FLYTHROUGH_MODE_ON, {}, {}));
	ASSERT_TRUE((m_controller.IsActive()));

	std::vector<IControllerMessageIDs> fileLoadMessages = {
		IDMENU_FIlE_TERRAIN_SHARP,
		IDMENU_FIlE_TERRAIN_SOFT,
		IDMENU_FIlE_CAMERA_TRAJECTORY,
		IDMENU_FIlE_CONFIGURATION
	};
	for (IControllerMessageIDs message : fileLoadMessages)
	{
		std::vector<std::wstring> path = { L"filepath" };
		EXPECT_CALL(*m_mock_model, HandleMessage(IDC2IDM(message), path, testing::_, testing::_)).Times(1).WillOnce(testing::Return(true));
		ASSERT_TRUE(m_controller.HandleMessage(message, {}, {}));
	}

	std::vector<IControllerMessageIDs> projectLoadMessages = {
		IDMENU_FIlE_TERRAIN_PROJECT_SHARP,
		IDMENU_FIlE_TERRAIN_PROJECT_SOFT,
	};
	for (IControllerMessageIDs message : projectLoadMessages)
	{
		std::vector<std::wstring> path = { L"path1", L"path2" };
		EXPECT_CALL(*m_mock_model, HandleMessage(IDC2IDM(message), path, testing::_, testing::_)).Times(1).WillOnce(testing::Return(true));
		ASSERT_TRUE(m_controller.HandleMessage(message, {}, {}));
	}

	//IDMENU_HELP
	EXPECT_CALL(*m_mock_view, ShowHelp()).Times(1);
	ASSERT_TRUE(m_controller.HandleMessage(IDMENU_HELP, {}, {}));

	//IDMENU_WINDOWS_EXPLORE3D
	EXPECT_CALL(*m_mock_view, ShowExplore3DWindow()).Times(0);
	ASSERT_FALSE(m_controller.HandleMessage(IDMENU_WINDOWS_EXPLORE3D, {}, {}));

	//IDMENU_WINDOWS_FLYTHROUGH
	EXPECT_CALL(*m_mock_view, ShowFlythroughWindow()).Times(0);
	ASSERT_FALSE(m_controller.HandleMessage(IDMENU_WINDOWS_FLYTHROUGH, {}, {}));

	//IDMENU_WINDOWS_GENERAL
	EXPECT_CALL(*m_mock_view, ShowGeneralWindow()).Times(1);
	ASSERT_TRUE(m_controller.HandleMessage(IDMENU_WINDOWS_GENERAL, {}, {}));

	//IDMENU_FILE_OUTPUT_DIRECTORY
	std::wstring dirpath = L"directoryPath";
	EXPECT_CALL(*m_mock_view, SetOutputDirectory(dirpath)).Times(1);
	ASSERT_TRUE(m_controller.HandleMessage(IDMENU_FILE_OUTPUT_DIRECTORY, {}, {}));

	std::vector<IControllerMessageIDs> messsages = {
		IDC_SET_CAMERA_FIELD_OF_VIEW,
		IDC_SET_CAMERA_NEAR_SCREEN,
		IDC_SET_CAMERA_FAR_SCREEN,
		IDC_MESH_SET_ISSEEN,
		IDC_MESH_SET_COLOR,
		IDC_MESH_GROUP_SCALE,
		IDC_MESH_GROUP_ROTATION,
		IDC_MESH_GROUP_TRANSLATION,
		IDC_TRAJECTORY_SET_ISSEEN,
		IDC_TRAJECTORY_ROTATION,
		IDC_TRAJECTORY_TRANSLATION,
		IDC_BUTTON_CLEAR_MESHES,
		IDC_SET_TIME_E3D,
		IDC_SET_START_TIME_TRAJECTORY,
		IDC_ORIGO_SET_LONGITUDE,
		IDC_ORIGO_SET_LATITUDE,
		IDC_XZ_PLANE_GRID_SET_ISSEEN,
		IDC_PIXELSHADER_SET_SHADING 
	};
	for (IControllerMessageIDs message : messsages)
	{
		EXPECT_CALL(*m_mock_model, HandleMessage(IDC2IDM(message), testing::_, testing::_, testing::_)).Times(1).WillOnce(testing::Return(true));
		ASSERT_TRUE(m_controller.HandleMessage(message, {}, {}));

	}

	




}