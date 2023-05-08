#include "Mocks.h"

class ControllerFlythroughTest : public ::testing::Test {
protected:
	std::shared_ptr<MockIModel>					m_mock_model;
	std::shared_ptr<MockIView>					m_mock_view;
	std::shared_ptr<MockKeyboard>				m_mock_keyboard;
	std::shared_ptr<MockMouse>					m_mock_mouse;
	ControllerFlythrough						m_controller;

	void SetUp() override {

		m_mock_model = std::make_shared<MockIModel>();
		m_mock_view = std::make_shared<MockIView>();
		m_mock_keyboard = std::make_shared<MockKeyboard>();
		m_mock_mouse = std::make_shared<MockMouse>();

		ASSERT_TRUE((m_controller.Initialize(m_mock_model, m_mock_view, m_mock_mouse, m_mock_keyboard)));
	}
};

TEST_F(ControllerFlythroughTest, Test_Initialize)
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

TEST_F(ControllerFlythroughTest, Test_HandleMessage)
{
	ASSERT_FALSE(m_controller.HandleMessage(IDCC_IS_FLYTHROUGH_MODE_ON, {}, {}));
	ASSERT_FALSE((m_controller.IsActive()));

	ASSERT_TRUE(m_controller.HandleMessage(IDC_ACTIVATE_3DEXPLORE_MODE, {}, {}));
	ASSERT_FALSE((m_controller.IsActive()));

	//IsTrajectoryInitialized returns false
	EXPECT_CALL(*m_mock_model, IsTrajectoryInitialized()).Times(1).WillOnce(testing::Return(false));
	ASSERT_FALSE(m_controller.HandleMessage(IDC_ACTIVATE_FLYTHROUGH_MODE, {}, {}));
	ASSERT_FALSE((m_controller.IsActive()));

	//IsTrajectoryInitialized returns true
	EXPECT_CALL(*m_mock_model, IsTrajectoryInitialized()).Times(1).WillOnce(testing::Return(true));
	EXPECT_CALL(*m_mock_model, HandleMessage(IDM_FLYTHROUGH_START_POSITION, testing::_, testing::_, testing::_)).Times(1);
	ASSERT_TRUE(m_controller.HandleMessage(IDC_ACTIVATE_FLYTHROUGH_MODE, {}, {}));
	ASSERT_TRUE((m_controller.IsActive()));


	//IDC_TIME_ELAPSED
	//Flythrough mode on but it is not running
	EXPECT_CALL(*m_mock_model, HandleMessage(testing::_, testing::_, testing::_, testing::_)).Times(0);
	EXPECT_CALL(*m_mock_view, HandleMessage(testing::_, testing::_, testing::_, testing::_)).Times(0);
	ASSERT_TRUE(m_controller.HandleMessage(IDC_TIME_ELAPSED, {}, {}));

	//start play trajectory
	EXPECT_CALL(*m_mock_model, HandleMessage(IDM_FLYTHROUGH_NEXT_FRAME, testing::_, testing::_, testing::_)).Times(1).WillOnce(testing::Return(true));;
	EXPECT_CALL(*m_mock_view, HandleMessage(testing::_, testing::_, testing::_, testing::_)).Times(0);
	ASSERT_TRUE(m_controller.HandleMessage(IDC_FLYTHROUGH_START, {}, {}));
	ASSERT_TRUE(m_controller.HandleMessage(IDC_TIME_ELAPSED, {}, {}));

	//Pause the trajectory playing
	EXPECT_CALL(*m_mock_model, HandleMessage(testing::_, testing::_, testing::_, testing::_)).Times(0);
	EXPECT_CALL(*m_mock_view, HandleMessage(testing::_, testing::_, testing::_, testing::_)).Times(0);
	ASSERT_TRUE(m_controller.HandleMessage(IDC_FLYTHROUGH_PAUSE, {}, {}));
	ASSERT_TRUE(m_controller.HandleMessage(IDC_TIME_ELAPSED, {}, {}));

	//stop the trajectory playing
	EXPECT_CALL(*m_mock_model, HandleMessage(IDM_FLYTHROUGH_START_POSITION, testing::_, testing::_, testing::_)).Times(1).WillOnce(testing::Return(true));
	EXPECT_CALL(*m_mock_view, HandleMessage(testing::_, testing::_, testing::_, testing::_)).Times(0);
	ASSERT_TRUE(m_controller.HandleMessage(IDC_FLYTHROUGH_STOP, {}, {}));
	ASSERT_TRUE(m_controller.HandleMessage(IDC_TIME_ELAPSED, {}, {}));

	//START RECORD without choose output directory
	EXPECT_CALL(*m_mock_model, HandleMessage(testing::_, testing::_, testing::_, testing::_)).Times(0);
	EXPECT_CALL(*m_mock_view, HandleMessage(testing::_, testing::_, testing::_, testing::_)).Times(0);
	ASSERT_FALSE(m_controller.HandleMessage(IDC_FLYTHROUGH_RECORD_START, {}, {}));
	ASSERT_TRUE(m_controller.HandleMessage(IDC_TIME_ELAPSED, {}, {}));

	//START RECORD withchoose output directory
	EXPECT_CALL(*m_mock_model, HandleMessage(IDM_FLYTHROUGH_NEXT_FRAME, testing::_, testing::_, testing::_)).Times(1).WillOnce(testing::Return(true));;
	EXPECT_CALL(*m_mock_view, HandleMessage(IDV_CAPTURE_SCREEN, testing::_, testing::_, testing::_)).Times(1);
	EXPECT_CALL(*m_mock_view, HandleMessage(IDV_FLYTHROUGH_RECORD_START, testing::_, testing::_, testing::_)).Times(1);
	ASSERT_TRUE(m_controller.HandleMessage(IDCC_OUTPUT_DIR_CHOOSED, {}, {}));
	ASSERT_TRUE(m_controller.HandleMessage(IDC_FLYTHROUGH_RECORD_START, {}, {}));
	ASSERT_TRUE(m_controller.HandleMessage(IDC_TIME_ELAPSED, {}, {}));


	//STOP RECORD
	EXPECT_CALL(*m_mock_model, HandleMessage(testing::_, testing::_, testing::_, testing::_)).Times(0);;
	EXPECT_CALL(*m_mock_view, HandleMessage(IDV_FLYTHROUGH_RECORD_STOP, testing::_, testing::_, testing::_)).Times(1);
	ASSERT_TRUE(m_controller.HandleMessage(IDC_FLYTHROUGH_RECORD_STOP, {}, {}));
	ASSERT_TRUE(m_controller.HandleMessage(IDC_TIME_ELAPSED, {}, {}));
}
