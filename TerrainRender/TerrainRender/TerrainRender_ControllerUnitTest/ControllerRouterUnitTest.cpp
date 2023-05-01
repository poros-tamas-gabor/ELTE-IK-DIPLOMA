#include "Mocks.h"

class ControllerRouterTest : public ::testing::Test {
protected:
	std::shared_ptr<MockIModel>					m_mock_model;
	std::shared_ptr<MockIView>					m_mock_view;
	std::shared_ptr<MockKeyboard>				m_mock_keyboard;
	std::shared_ptr<MockMouse>					m_mock_mouse;
	ControllerRouter							m_controller;

	void SetUp() override {

		m_mock_model	= std::make_shared<MockIModel>();
		m_mock_view		= std::make_shared<MockIView>();
		m_mock_keyboard = std::make_shared<MockKeyboard>();
		m_mock_mouse	= std::make_shared<MockMouse>();

		ASSERT_TRUE((m_controller.Initialize(m_mock_model, m_mock_view, m_mock_mouse, m_mock_keyboard)));
	}
};

TEST_F(ControllerRouterTest, Initialize)
{
	IModelPtr									mock_model;
	IViewPtr									mock_view;
	KeyboardPtr									mock_keyboard;
	MousePtr									mock_mouse;
	ControllerRouter							controller;


	ASSERT_FALSE((controller.Initialize(mock_model, m_mock_view, m_mock_mouse, m_mock_keyboard)));
	ASSERT_FALSE((controller.Initialize(m_mock_model, mock_view, m_mock_mouse, m_mock_keyboard)));
	ASSERT_FALSE((controller.Initialize(m_mock_model, m_mock_view, mock_mouse, m_mock_keyboard)));
	ASSERT_FALSE((controller.Initialize(m_mock_model, m_mock_view, m_mock_mouse, mock_keyboard)));

	ASSERT_TRUE((controller.Initialize(m_mock_model, m_mock_view, m_mock_mouse, m_mock_keyboard)));
	ASSERT_TRUE((controller.IsActive()));
}

TEST_F(ControllerRouterTest, AddController_RemoveController)
{

	std::shared_ptr<MockController>	 controller1;

	ASSERT_FALSE(m_controller.AddController(controller1));

	controller1 = std::make_shared<MockController>();

	EXPECT_CALL(*controller1, SetMessageSystem(testing::_)).Times(1);
	ASSERT_TRUE(m_controller.AddController(controller1));

	ASSERT_FALSE(m_controller.AddController(controller1));

	ASSERT_TRUE(m_controller.RemoveController(controller1));
	ASSERT_FALSE(m_controller.RemoveController(controller1));
}

TEST_F(ControllerRouterTest, HandleMessage)
{

	std::shared_ptr<MockController>	 controller1;
	controller1 = std::make_shared<MockController>();

	EXPECT_CALL(*controller1, SetMessageSystem(testing::_)).Times(1);
	ASSERT_TRUE(m_controller.AddController(controller1));

	EXPECT_CALL(*controller1, HandleMessage(IDC_TIME_ELAPSED, std::vector<float>{}, std::vector<unsigned>{})).Times(1);
	m_controller.HandleMessage(IDC_TIME_ELAPSED, {}, {});
}
