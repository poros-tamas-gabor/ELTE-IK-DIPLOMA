#ifndef CAMERA_TRAJECTORY_H
#define CAMERA_TRAJECTORY_H

///////////////////////////////////////////////////////////////////////////////
// CameraTrajectory.h
// ==================
// 
// In the Flythrough mode, the camera handling is implemented by the CameraTrajectory class.
// The responsibility of the class is to handle the camera trajectory file.
// The CameraTrajectory object refers to the polyline created during the loading of the trajectory file, which represents the camera path to the user.
// 
// In the Flythrough mode, the CameraTrajectory::UpdateCamera(double dt) function receives the time elapsed between the two frames in milliseconds.
// During the simulation playback, the current position and rotation angles can be determined with two separate functions, which calculate the function value based on the elapsed time.
// 
// The elapsed time is stored in the double m_elapsedmsec member variable, which represents the total time elapsed since the start time.
// 
// During the interpolation process, we approximate the direct points linearly, which we know from the camera trajectory file.
// This allows us to determine the function value for any t parameter that falls within the start and end time interval.
//
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

#include "IRenderable.h"
#include "Persistence/ModelStructs.h"
#include "Math/Interpolations.h"
#include "Persistence/EpochTime.h"
#include <DirectXMath.h>
#include <vector>
#include <ctime>
#include "Camera.h"

class CameraTrajectory 
{
private:

	EpochTime								m_start;
	float									m_elapsedmsec		= 0.0f;
	unsigned								m_currentFrameNum	= 0;
	std::vector<float>						m_elapsedmsecs;
	std::vector<Vector3D>					m_positions;
	std::vector<Vector3D>					m_rotations;
	IRendarablePtr<VertexPolyLine> 			m_polyLine;
	CameraPtr								m_camera;
	float									m_speed = 1.0f;


public:
	bool Initialize(const std::vector<CameraPose>& cameraPoses, IRendarablePtr<VertexPolyLine>  renderable, CameraPtr camera);
	bool UpdateCamera(float elapsedmsecs);
	void ResetStartPosition(void);
	void Clear(void);
	void Shutdown(void);

	void SetStartEpochTime(EpochTime);
	void SetCurrentFrame(unsigned frameNum);
	void SetSpeed(float speed);

	bool		IsInitialized(void) const;
	float		GetSpeed(void) const;
	EpochTime	GetCurrentEpochTime(void) const;
	EpochTime	GetStartEpochTime(void) const;
	unsigned	GetCurrentFrameNum(void) const;
	unsigned	GetNumberOfFrame(void) const;
	IRenderableState				GetTrajectoryPolyLineState() const;
	IRendarablePtr<VertexPolyLine>	GetPolyLine() const;

private:
	Vector3D TransformPosition(const Vector3D&) const;
	Vector3D TransformRotation(const Vector3D&) const;
};
#endif // !CAMERA_TRAJECTORY_H
