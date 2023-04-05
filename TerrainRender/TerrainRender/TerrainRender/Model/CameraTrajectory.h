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
	double									m_elapsedmsec;
	std::vector<double>						m_elapsedmsecs;
	std::vector<Vector3D>					m_positions;
	std::vector<Vector3D>					m_rotations;
	unsigned								m_currentFrameNum;
	IRendarablePtr<VertexPolyLine> 			m_renderable;
	Camera*									m_camera;


public:
	bool Initialize(const std::vector<CameraPose>& cameraPoses, IRendarablePtr<VertexPolyLine>  renderable, Camera* camera);

	void UpdateCamera(double elapsedmsecs);
	void Reset();
	void Shutdown();

	EpochTime	GetCurrentEpochTime(void) const;
	unsigned	GetCurrentFrameNum(void) const;
	unsigned	GetNumberOfFrame(void) const;
	void		SetCurrentFrame(unsigned frameNum);

private:
	Vector3D TransformPosition(const Vector3D&) const;
	Vector3D TransformRotation(const Vector3D&) const;



};
#endif // !CAMERA_TRAJECTORY_H
