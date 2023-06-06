#include "Mocks.h"

class ControllerExplore3DTest : public ::testing::Test {
protected:
	std::shared_ptr<MockIModel>					m_mock_model;
	std::shared_ptr<MockIView>					m_mock_view;
	std::shared_ptr<MockKeyboard>				m_mock_keyboard;
	std::shared_ptr<MockMouse>					m_mock_mouse;
	Controller3DExplore							m_controller;

	void SetUp() override {

		m_mock_model = std::make_shared<MockIModel>();
		m_mock_view = std::make_shared<MockIView>();
		m_mock_keyboard = std::make_shared<MockKeyboard>();
		m_mock_mouse = std::make_shared<MockMouse>();

		ASSERT_TRUE((m_controller.Initialize(m_mock_model, m_mock_view, m_mock_mouse, m_mock_keyboard)));
	}
};

TEST_F(ControllerExplore3DTest, Test_Initialize)
{
	IModelPtr									mock_model;
	IViewPtr									mock_view;
	KeyboardPtr									mock_keyboard;
	MousePtr									mock_mouse;
	Controller3DExplore							controller;


	ASSERT_FALSE((controller.Initialize(mock_model, m_mock_view, m_mock_mouse, m_mock_keyboard)));
	ASSERT_FALSE((controller.Initialize(m_mock_model, mock_view, m_mock_mouse, m_mock_keyboard)));
	ASSERT_FALSE((controller.Initialize(m_mock_model, m_mock_view, mock_mouse, m_mock_keyboard)));
	ASSERT_FALSE((controller.Initialize(m_mock_model, m_mock_view, m_mock_mouse, mock_keyboard)));

	ASSERT_TRUE((controller.Initialize(m_mock_model, m_mock_view, m_mock_mouse, m_mock_keyboard)));
	ASSERT_TRUE((controller.IsActive()));
}

TEST_F(ControllerExplore3DTest, Test_HandleMessage)
{
	ASSERT_FALSE(m_controller.HandleMessage(IDCC_IS_FLYTHROUGH_MODE_ON, {}, {}));
	ASSERT_TRUE((m_controller.IsActive()));

	//IDC_ACTIVATE_FLYTHROUGH_MODE
	EXPECT_CALL(*m_mock_model, IsTrajectoryInitialized()).Times(1).WillOnce(testing::Return(true));
	ASSERT_TRUE(m_controller.HandleMessage(IDC_ACTIVATE_FLYTHROUGH_MODE, {}, {}));
	ASSERT_FALSE((m_controller.IsActive()));

	//IDC_ACTIVATE_EXPLORE3D_MODE
	EXPECT_CALL(*m_mock_model, HandleMessage(IDM_ACTIVATE_EXPLORE3D_MODE, testing::_, testing::_, testing::_)).Times(1).WillOnce(testing::Return(true));
	ASSERT_TRUE(m_controller.HandleMessage(IDC_ACTIVATE_EXPLORE3D_MODE, {}, {}));
	ASSERT_TRUE((m_controller.IsActive()));

	//IDC_E3D_CAMERA_SPEED:
	//IDC_E3D_ROTATION_SPEED:
	//IDC_E3D_CAMERA_RESET:
	std::vector<IControllerMessageIDs> messages = { IDC_E3D_CAMERA_SPEED , IDC_E3D_ROTATION_SPEED, IDC_E3D_CAMERA_RESET };
	for (IControllerMessageIDs message : messages)
	{
		EXPECT_CALL(*m_mock_model, HandleMessage(IDC2IDM(message), testing::_, testing::_, testing::_)).Times(1).WillOnce(testing::Return(true));
		ASSERT_TRUE(m_controller.HandleMessage(message, {}, {}));
	}

	//IDC_TIME_ELAPSED
	//SPACE key down
	//EXPECT_CALL(*m_mock_keyboard, KeyIsPressed(VK_SPACE)).Times(1).WillOnce(testing::Return(true));
	//EXPECT_CALL(*m_mock_keyboard, KeyBufferIsEmpty()).Times(1).WillOnce(testing::Return(true));
	//EXPECT_CALL(*m_mock_keyboard, CharBufferIsEmpty()).Times(1).WillOnce(testing::Return(true));
	//EXPECT_CALL(*m_mock_mouse, EventBufferIsEmpty()).Times(1).WillOnce(testing::Return(true));
	//EXPECT_CALL(*m_mock_model, HandleMessage(IDM_E3D_MOVE_UP, testing::_, testing::_, testing::_)).Times(1).WillOnce(testing::Return(true));
	//ASSERT_TRUE((m_controller.IsActive()));
	//ASSERT_TRUE(m_controller.HandleMessage(IDC_TIME_ELAPSED, {}, {}));


}