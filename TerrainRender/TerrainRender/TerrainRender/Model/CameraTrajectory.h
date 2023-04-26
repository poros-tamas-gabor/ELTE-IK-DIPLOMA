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
	IRendarablePtr<VertexPolyLine> 			m_polyLine;
	CameraPtr								m_camera;


public:
	bool Initialize(const std::vector<CameraPose>& cameraPoses, IRendarablePtr<VertexPolyLine>  renderable, CameraPtr camera);

	bool IsInitialized(void) const;
	void UpdateCamera(double elapsedmsecs);
	void Reset();
	void Shutdown();
	void Clear();

	void Rotate(Vector3D rotations);
	void Move(Vector3D   translationVector);

	void			SetStartEpochTime(EpochTime);
	EpochTime		GetCurrentEpochTime(void) const;
	EpochTime		GetStartEpochTime(void) const;
	unsigned		GetCurrentFrameNum(void) const;
	unsigned		GetNumberOfFrame(void) const;
	void			SetCurrentFrame(unsigned frameNum);
	IRenderableState GetTrajectoryPolyLineState() const;
	IRendarablePtr<VertexPolyLine> GetPolyLine() const;

private:
	Vector3D TransformPosition(const Vector3D&) const;
	Vector3D TransformRotation(const Vector3D&) const;



};
#endif // !CAMERA_TRAJECTORY_H
