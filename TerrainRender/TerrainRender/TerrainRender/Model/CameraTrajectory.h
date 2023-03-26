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
	IRendarablePtr<VertexPolyLine> 			m_renderable;
	Camera*									m_camera;
	LinearInterpolation<double, Vector3D>	m_interpolation;

public:
	bool Initialize(const std::vector<CameraPose>& cameraPoses, IRendarablePtr<VertexPolyLine>  renderable, Camera* camera);

	void UpdateCamera(double elapsedmsecs);
	void Reset();
	EpochTime GetCurrentEpochTime(void) const;


	void Shutdown();

};
#endif // !CAMERA_TRAJECTORY_H
