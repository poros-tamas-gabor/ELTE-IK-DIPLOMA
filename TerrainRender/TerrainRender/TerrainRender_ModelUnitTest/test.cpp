#include "pch.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}



class MockTest {
public:
    MOCK_METHOD(void, SomeMethod, ());
};

TEST(TestCaseName, TestName2) {
    MockTest mock;
    EXPECT_CALL(mock, SomeMethod);
    mock.SomeMethod();
    EXPECT_EQ(1, 1);
    EXPECT_TRUE(true);
}