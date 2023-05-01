#ifndef CAMERA_TRAJECTORY_H
#define CAMERA_TRAJECTORY_H

#include "IRenderable.h"
#include "Persistence/ModelStructs.h"
#include "Math/IInterpolation.h"
#include "Persistence/EpochTime.h"
#include <DirectXMath.h>
#include <vector>
#include <ctime>
#include "Camera.h"


class CameraTrajectory 
{
private:

	EpochTime								m_start;
	float									m_elapsedmsec = 0.0;
	std::vector<float>						m_elapsedmsecs;
	std::vector<Vector3D>					m_positions;
	std::vector<Vector3D>					m_rotations;
	unsigned								m_currentFrameNum = 0;
	IRendarablePtr<VertexPolyLine> 			m_polyLine;
	CameraPtr								m_camera;
	float									m_speed = 1.0f;


public:
	bool Initialize(const std::vector<CameraPose>& cameraPoses, IRendarablePtr<VertexPolyLine>  renderable, CameraPtr camera);

	bool IsInitialized(void) const;
	bool UpdateCamera(float elapsedmsecs);
	void ResetStartPosition();
	void Clear();
	void Shutdown();

	void SetStartEpochTime(EpochTime);
	void SetCurrentFrame(unsigned frameNum);
	void SetSpeed(float speed);

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
