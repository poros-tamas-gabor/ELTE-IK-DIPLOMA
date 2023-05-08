#include "pch.h"
#include "Mocks.h"

class CameraTrajectoryTest : public ::testing::Test {
protected:
    CameraPtr							m_camera;
    CameraTrajectory					m_trajectory;
	IRendarablePtr<VertexPolyLine>		m_polyline;
    std::vector<CameraPose>				m_cameraPoses;

    void SetUp() override {

		CameraPose cameraPose;

        m_camera = std::make_shared<Camera>();
		m_polyline = std::make_shared<PolyLine>();
	
		cameraPose = { {0, 0}, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
		m_cameraPoses.push_back(cameraPose);

		cameraPose = { {1, 0}, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f };
		m_cameraPoses.push_back(cameraPose);

		cameraPose = { {2, 0}, 0.0f, 0.0f, 1.5f, 2.0f, 0.0f, 0.0f };
		m_cameraPoses.push_back(cameraPose);

		cameraPose = { {3, 0}, 0.0f, 1.0f, 1.5f, 2.0f, 1.0f, 0.0f };
		m_cameraPoses.push_back(cameraPose);

		cameraPose = { {4, 0}, 0.0f, 1.0f, 1.5f, 2.0f, 2.0f, 0.0f };
		m_cameraPoses.push_back(cameraPose);

		cameraPose = { {5, 0}, -1.0f, 1.0f, 1.5f, 2.0f, 2.0f, -1.0f };
		m_cameraPoses.push_back(cameraPose);

		ASSERT_TRUE(m_trajectory.Initialize(m_cameraPoses, m_polyline, m_camera));


    }
};

TEST_F(CameraTrajectoryTest, Test_InitializeInvalid)
{
	CameraTrajectory				trajectory;
	CameraPtr						nullCamera;
	IRendarablePtr<VertexPolyLine>	nullRenderable;
	std::vector<CameraPose>			emptyCameraPoses;

	ASSERT_FALSE(trajectory.IsInitialized());
	ASSERT_THROW(trajectory.Initialize(emptyCameraPoses, m_polyline, m_camera), TRException);

	ASSERT_FALSE(trajectory.IsInitialized());
	ASSERT_THROW(trajectory.Initialize(m_cameraPoses, nullRenderable, m_camera), TRException);

	ASSERT_FALSE(trajectory.IsInitialized());
	ASSERT_THROW(trajectory.Initialize(m_cameraPoses, m_polyline, nullCamera), TRException);

	ASSERT_FALSE(trajectory.IsInitialized());
}
TEST_F(CameraTrajectoryTest, Test_Initialize)
{
	CameraTrajectory				trajectory;

	ASSERT_TRUE(trajectory.Initialize(m_cameraPoses, m_polyline, m_camera));
	ASSERT_TRUE(trajectory.IsInitialized());

	trajectory.Clear();
	ASSERT_FALSE(trajectory.IsInitialized());

	ASSERT_EQ(m_trajectory.GetCurrentEpochTime(), EpochTime(0, 0));
	ASSERT_EQ(m_trajectory.GetCurrentFrameNum(), 0);
	ASSERT_EQ(m_trajectory.GetNumberOfFrame(), 6);
	ASSERT_EQ(m_trajectory.GetStartEpochTime(), EpochTime(0, 0));
}

TEST_F(CameraTrajectoryTest, Test_UpdateCamera)
{
	m_trajectory.ResetStartPosition();
	ASSERT_TRUE(m_trajectory.UpdateCamera(0));
	
	ASSERT_EQ(m_trajectory.GetCurrentEpochTime(), EpochTime(0, 0));
	ASSERT_EQ(m_trajectory.GetCurrentFrameNum(), 0);
	ASSERT_EQ(m_trajectory.GetNumberOfFrame(), 6);
	ASSERT_EQ(m_trajectory.GetStartEpochTime(), EpochTime(0, 0));
	ASSERT_EQ(m_camera->GetPositionVec(), Vector3D(0, 0, 0));
	ASSERT_EQ(m_camera->GetRotationVec(), Vector3D(0, 0, 0));

	ASSERT_TRUE(m_trajectory.UpdateCamera(500));
	
	ASSERT_EQ(m_trajectory.GetCurrentEpochTime(), EpochTime(0, 500 * EpochTime::NSEC_PER_MSEC));
	ASSERT_EQ(m_trajectory.GetCurrentFrameNum(), 0);
	ASSERT_EQ(m_camera->GetPositionVec(), Vector3D(0, 0, +0.5f));
	ASSERT_EQ(m_camera->GetRotationVec(), Vector3D(0, 0, -0.5f));

	ASSERT_TRUE(m_trajectory.UpdateCamera(500));

	ASSERT_EQ(m_trajectory.GetCurrentEpochTime(), EpochTime(0, 1000 * EpochTime::NSEC_PER_MSEC));
	ASSERT_EQ(m_trajectory.GetCurrentFrameNum(), 1);
	ASSERT_EQ(m_camera->GetPositionVec(), Vector3D(0, 0, +1.0f));
	ASSERT_EQ(m_camera->GetRotationVec(), Vector3D(0, 0, -1.0f));

	ASSERT_TRUE(m_trajectory.UpdateCamera(4000));

	ASSERT_EQ(m_trajectory.GetCurrentEpochTime(), EpochTime(5, 0));
	ASSERT_EQ(m_trajectory.GetCurrentFrameNum(), 5);


	ASSERT_EQ(m_camera->GetPositionVec(), Vector3D(2.0f, +1.0f, +2.0f));
	Vector3D expectedRot = { -1.0f, -1.0f, -1.5f };
	ASSERT_FLOAT_EQ(m_camera->GetRotationVec().x, expectedRot.x);
	ASSERT_FLOAT_EQ(m_camera->GetRotationVec().y, expectedRot.y);
	ASSERT_FLOAT_EQ(m_camera->GetRotationVec().z, expectedRot.z);

	ASSERT_FALSE(m_trajectory.UpdateCamera(1000));
}