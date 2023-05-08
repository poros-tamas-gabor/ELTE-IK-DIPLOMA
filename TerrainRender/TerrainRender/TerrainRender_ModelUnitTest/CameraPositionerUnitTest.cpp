#include "pch.h"

class CameraPositionerTest : public ::testing::Test {
protected:
    CameraPtr           m_camera;
    CameraPositioner    m_positioner;

    void SetUp() override {
        m_camera = std::make_shared<Camera>();
        m_positioner.Initialize(m_camera);
    }
};

TEST_F(CameraPositionerTest, Test_InitializeInvalid)
{
    CameraPtr           nullCamera;
    CameraPositioner    positioner;

    ASSERT_THROW(positioner.Initialize(nullCamera), TRException);
}

TEST_F(CameraPositionerTest, Test_CurrentEpochTime)
{
    const EpochTime epochtime = 1234567890;
    m_positioner.SetCurrentEpochTime(epochtime);
    ASSERT_EQ(m_positioner.GetCurrentEpochTime(), epochtime);
}

TEST_F(CameraPositionerTest, Test_MoveForward)
{
    Vector3D    expected;
    const float elapsedTime = 1.0f;
    float       speed       = 1.0f;

    m_camera->SetPosition(0.0f, 0.0f, 0.0f);

    m_positioner.SetSpeed(speed);
    m_positioner.MoveForward(elapsedTime);

    expected = { 0.0f, 0.0f, 1.0f };
    ASSERT_EQ(m_camera->GetPositionVec(), expected);

    speed = 2.0f;
    m_positioner.SetSpeed(speed);
    m_positioner.MoveForward(elapsedTime);

    expected = { 0.0f, 0.0f, 3.0f };
    ASSERT_EQ(m_camera->GetPositionVec(), expected);

    speed = 0.0f;
    m_positioner.SetSpeed(speed);
    m_positioner.MoveForward(elapsedTime);

    expected = { 0.0f, 0.0f, 3.0f };
    ASSERT_EQ(m_camera->GetPositionVec(), expected);
}

TEST_F(CameraPositionerTest, Test_MoveBack)
{
    Vector3D    expected;
    const float elapsedTime = 1.0f;
    float       speed = 1.0f;

    m_camera->SetPosition(0.0f, 0.0f, 0.0f);

    m_positioner.SetSpeed(speed);
    m_positioner.MoveBack(elapsedTime);

    expected = { 0.0f, 0.0f, -1.0f };
    ASSERT_EQ(m_camera->GetPositionVec(), expected);

    speed = 2.0f;
    m_positioner.SetSpeed(speed);
    m_positioner.MoveBack(elapsedTime);

    expected = { 0.0f, 0.0f, -3.0f };
    ASSERT_EQ(m_camera->GetPositionVec(), expected);

    speed = 0.0f;
    m_positioner.SetSpeed(speed);
    m_positioner.MoveBack(elapsedTime);

    expected = { 0.0f, 0.0f, -3.0f };
    ASSERT_EQ(m_camera->GetPositionVec(), expected);
}

TEST_F(CameraPositionerTest, Test_MoveLeft)
{
    Vector3D    expected;
    const float elapsedTime = 1.0f;
    float       speed = 1.0f;

    m_camera->SetPosition(0.0f, 0.0f, 0.0f);

    m_positioner.SetSpeed(speed);
    m_positioner.MoveLeft(elapsedTime);

    expected = { -1.0f, 0.0f, 0.0f };
    ASSERT_EQ(m_camera->GetPositionVec(), expected);

    speed = 2.0f;
    m_positioner.SetSpeed(speed);
    m_positioner.MoveLeft(elapsedTime);

    expected = { -3.0f, 0.0f, 0.0f };
    ASSERT_EQ(m_camera->GetPositionVec(), expected);

    speed = 0.0f;
    m_positioner.SetSpeed(speed);
    m_positioner.MoveLeft(elapsedTime);

    expected = { -3.0f, 0.0f, 0.0f };
    ASSERT_EQ(m_camera->GetPositionVec(), expected);
}

TEST_F(CameraPositionerTest, Test_MoveRight)
{
    Vector3D    expected;
    const float elapsedTime = 1.0f;
    float       speed = 1.0f;

    m_camera->SetPosition(0.0f, 0.0f, 0.0f);

    m_positioner.SetSpeed(speed);
    m_positioner.MoveRight(elapsedTime);

    expected = { +1.0f, 0.0f, 0.0f };
    ASSERT_EQ(m_camera->GetPositionVec(), expected);

    speed = 2.0f;
    m_positioner.SetSpeed(speed);
    m_positioner.MoveRight(elapsedTime);

    expected = { +3.0f, 0.0f, 0.0f };
    ASSERT_EQ(m_camera->GetPositionVec(), expected);

    speed = 0.0f;
    m_positioner.SetSpeed(speed);
    m_positioner.MoveRight(elapsedTime);

    expected = { +3.0f, 0.0f, 0.0f };
    ASSERT_EQ(m_camera->GetPositionVec(), expected);
}

TEST_F(CameraPositionerTest, Test_MoveUp)
{
    Vector3D    expected;
    const float elapsedTime = 1.0f;
    float       speed = 1.0f;

    m_camera->SetPosition(0.0f, 0.0f, 0.0f);

    m_positioner.SetSpeed(speed);
    m_positioner.MoveUp(elapsedTime);

    expected = { 0.0f, +1.0f, 0.0f };
    ASSERT_EQ(m_camera->GetPositionVec(), expected);

    speed = 2.0f;
    m_positioner.SetSpeed(speed);
    m_positioner.MoveUp(elapsedTime);

    expected = { 0.0f, +3.0f, 0.0f };
    ASSERT_EQ(m_camera->GetPositionVec(), expected);

    speed = 0.0f;
    m_positioner.SetSpeed(speed);
    m_positioner.MoveUp(elapsedTime);

    expected = { 0.0f, +3.0f, 0.0f };
    ASSERT_EQ(m_camera->GetPositionVec(), expected);
}

TEST_F(CameraPositionerTest, Test_MoveDown)
{
    Vector3D    expected;
    const float elapsedTime = 1.0f;
    float       speed = 1.0f;

    m_camera->SetPosition(0.0f, 0.0f, 0.0f);

    m_positioner.SetSpeed(speed);
    m_positioner.MoveDown(elapsedTime);

    expected = { 0.0f, -1.0f, 0.0f };
    ASSERT_EQ(m_camera->GetPositionVec(), expected);

    speed = 2.0f;
    m_positioner.SetSpeed(speed);
    m_positioner.MoveDown(elapsedTime);

    expected = { 0.0f, -3.0f, 0.0f };
    ASSERT_EQ(m_camera->GetPositionVec(), expected);

    speed = 0.0f;
    m_positioner.SetSpeed(speed);
    m_positioner.MoveDown(elapsedTime);

    expected = { 0.0f, -3.0f, 0.0f };
    ASSERT_EQ(m_camera->GetPositionVec(), expected);
}

TEST_F(CameraPositionerTest, Test_Rotate)
{
    Vector3D    expected;
    const float elapsedTime = 1.0f;
    float       speed = 1.0f;

    m_positioner.SetRotationSpeed(1.0f);
    m_camera->SetRotationRad(0.0f, 0.0f, 0.0f);

    m_positioner.RotatePitchYaw(1.0f, 1.0f);
    expected = { 1.0f, 1.0f, 0.0f };
    ASSERT_EQ(m_camera->GetRotationVec(), expected);

    m_positioner.RotatePitchYaw(1.0f, 1.0f);
    expected = { 2.0f, 2.0f, 0.0f };
    ASSERT_EQ(m_camera->GetRotationVec(), expected);

    m_positioner.RotatePitchYaw(0.0f, 0.0f);
    expected = { 2.0f, 2.0f, 0.0f };
    ASSERT_EQ(m_camera->GetRotationVec(), expected);

    m_positioner.RotatePitchYaw(-1.0f, -1.0f);
    expected = { 1.0f, 1.0f, 0.0f };
    ASSERT_EQ(m_camera->GetRotationVec(), expected);

    m_positioner.RotatePitchYaw(-1.0f, -1.0f);
    expected = { 0.0f, 0.0f, 0.0f };
    ASSERT_EQ(m_camera->GetRotationVec(), expected);
}