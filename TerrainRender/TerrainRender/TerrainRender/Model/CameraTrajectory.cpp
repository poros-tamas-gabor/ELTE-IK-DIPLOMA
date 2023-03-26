#include "CameraTrajectory.h"

bool CameraTrajectory::Initialize(const std::vector<CameraPose>& cameraPoses, IRendarablePtr<VertexPolyLine> renderable, Camera* camera)
{
	if (renderable == nullptr || camera == nullptr)
	{
		return false;
	}
	this->m_elapsedmsecs.clear();
	this->m_positions.clear();
	this->m_renderable = NULL;
	this->m_rotations.clear();
	this->m_elapsedmsec = 0;

	this->m_camera = camera;
	this->m_start = cameraPoses.at(0).epochtime;
	for (const CameraPose& camerapose : cameraPoses)
	{
		double elapsedMsec = camerapose.epochtime.diffInMillis(m_start);
		this->m_elapsedmsecs.push_back(elapsedMsec);
		this->m_positions.push_back({ (float)camerapose.east,-(float)camerapose.down,(float)camerapose.north });
		this->m_rotations.push_back({ -(float)camerapose.pitch, (float)camerapose.yaw, -(float)camerapose.roll });
	}
	this->m_renderable = renderable;	
	return true;
} 

void CameraTrajectory::Shutdown()
{}

void CameraTrajectory::Reset()
{
	this->m_elapsedmsec = 0;
	this->m_camera->SetPosition(m_positions.at(0).x, m_positions.at(0).y, m_positions.at(0).z);
	this->m_camera->SetRotationRad(m_rotations.at(0).x, m_rotations.at(0).y, m_rotations.at(0).z);

}
void CameraTrajectory::UpdateCamera(double elapsedmsec)
{
	this->m_elapsedmsec += elapsedmsec;

	bool result;
	Vector3D currentPosition;
	result = m_interpolation.Calculate(this->m_elapsedmsecs, this->m_positions, this->m_elapsedmsec, currentPosition);
	if (!result)
		return;
	Vector3D currentRotation;
	result = m_interpolation.Calculate(this->m_elapsedmsecs, this->m_rotations, this->m_elapsedmsec, currentRotation);
	if (!result)
		return;
	
	this->m_camera->SetPosition(currentPosition.x, currentPosition.y, currentPosition.z);
	this->m_camera->SetRotationRad(currentRotation.x, currentRotation.y, currentRotation.z);
}

EpochTime CameraTrajectory::GetCurrentEpochTime(void) const
{
	return this->m_start.AddMilliSeconds(m_elapsedmsec);
}


