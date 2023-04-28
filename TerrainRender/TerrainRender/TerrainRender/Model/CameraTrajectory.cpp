#include "CameraTrajectory.h"
#include "../ErrorHandler.h"

bool CameraTrajectory::Initialize(const std::vector<CameraPose>& cameraPoses, IRendarablePtr<VertexPolyLine> renderable, CameraPtr camera)
{
	THROW_TREXCEPTION_IF_FAILED((renderable != nullptr), L"Failed to initialize CameraTrajectory because the renderable is a nullpointer");
	THROW_TREXCEPTION_IF_FAILED((camera != nullptr), L"Failed to initialize CameraTrajectory because the camera is a nullpointer");
	THROW_TREXCEPTION_IF_FAILED((!cameraPoses.empty()), L"Failed to initialize CameraTrajectory because the cameraPoses are empty");

	Clear();

	this->m_camera = camera;
	this->m_start = cameraPoses.at(0).epochtime;
	for (const CameraPose& camerapose : cameraPoses)
	{
		double elapsedMsec = camerapose.epochtime.diffInMillis(m_start);
		this->m_elapsedmsecs.push_back(elapsedMsec);
		this->m_positions.push_back({ (float)camerapose.east,-(float)camerapose.down,(float)camerapose.north });
		this->m_rotations.push_back({ -(float)camerapose.pitch, (float)camerapose.yaw, -(float)camerapose.roll });
	}
	this->m_polyLine = renderable;	
	return true;
} 


bool CameraTrajectory::IsInitialized() const
{
	return m_rotations.size() != 0 && m_elapsedmsecs.size() != 0 && m_positions.size() != 0 && m_polyLine.get() != nullptr;
}
void CameraTrajectory::Shutdown()
{}

void CameraTrajectory::Clear()
{
	this->m_elapsedmsecs.clear();
	this->m_positions.clear();
	this->m_rotations.clear();
	this->m_elapsedmsec = 0;
	m_currentFrameNum = 0;
	this->m_polyLine.reset();
}

IRendarablePtr<VertexPolyLine> CameraTrajectory::GetPolyLine() const
{
	return m_polyLine;
}

void CameraTrajectory::ResetStartPosition()
{
	THROW_TREXCEPTION_IF_FAILED((m_camera != nullptr), L"Null pointer exception");

	Vector3D currentCameraRotation = m_rotations.at(0);
	Vector3D currentCameraPosition = m_positions.at(0); 
	
	currentCameraPosition = TransformPosition(currentCameraPosition);
	currentCameraRotation = TransformRotation(currentCameraRotation);

	this->m_elapsedmsec = 0;
	this->m_camera->SetPosition(currentCameraPosition.x, currentCameraPosition.y, currentCameraPosition.z);
	this->m_camera->SetRotationRad(currentCameraRotation.x, currentCameraRotation.y, currentCameraRotation.z);
}

Vector3D  CameraTrajectory::TransformPosition(const Vector3D& vector) const
{
	THROW_TREXCEPTION_IF_FAILED((m_polyLine != nullptr), L"Null pointer exception");

	DirectX::XMMATRIX rotation = m_polyLine->GetWorldMatrix();
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(vector.x, vector.y, vector.z, 1.0f);
	vec = DirectX::XMVector4Transform(vec, rotation);
	DirectX::XMFLOAT3 vecfloat3;
	DirectX::XMStoreFloat3(&vecfloat3, vec);
	return { vecfloat3.x,vecfloat3.y,vecfloat3.z };

}
Vector3D  CameraTrajectory::TransformRotation(const Vector3D& vector) const
{
	THROW_TREXCEPTION_IF_FAILED((m_polyLine != nullptr), L"Null pointer exception");

	DirectX::XMMATRIX polylineWorldMat = m_polyLine->GetWorldMatrix();
	DirectX::XMMATRIX cameraRotMat = DirectX::XMMatrixRotationRollPitchYaw(vector.x, vector.y, vector.z);

	DirectX::XMMATRIX mat = cameraRotMat * polylineWorldMat;

	DirectX::XMFLOAT4X4 XMFLOAT4X4_Values;
	DirectX::XMStoreFloat4x4(&XMFLOAT4X4_Values, DirectX::XMMatrixTranspose(mat));
	float pitch = (float)asin(-XMFLOAT4X4_Values._23);
	float yaw = (float)atan2(XMFLOAT4X4_Values._13, XMFLOAT4X4_Values._33);
	float roll = (float)atan2(XMFLOAT4X4_Values._21, XMFLOAT4X4_Values._22);
	return { pitch,yaw, roll };
}

bool CameraTrajectory::UpdateCamera(double elapsedmsec)
{
	THROW_TREXCEPTION_IF_FAILED((m_camera != nullptr), L"Null pointer exception");

	elapsedmsec *= m_speed;

	bool result;
	Vector3D currentCameraRotation;
	Vector3D currentCameraPosition;

	this->m_elapsedmsec += elapsedmsec;

	LinearInterpolation	linearInterpolation;
	result = linearInterpolation.Calculate(this->m_elapsedmsecs, this->m_positions, this->m_elapsedmsec, currentCameraPosition, m_currentFrameNum);
	if (!result)
		return false;

	CirclularInterpolation circularInterpolation;
	result = circularInterpolation.Calculate(this->m_elapsedmsecs, this->m_rotations, this->m_elapsedmsec, currentCameraRotation, m_currentFrameNum);
	if (!result)
		return false;

	currentCameraRotation = TransformRotation(currentCameraRotation);
	currentCameraPosition = TransformPosition(currentCameraPosition);
	
	this->m_camera->SetPosition(currentCameraPosition.x, currentCameraPosition.y, currentCameraPosition.z);
	this->m_camera->SetRotationRad(currentCameraRotation.x, currentCameraRotation.y, currentCameraRotation.z);

	return true;
}

void CameraTrajectory::SetStartEpochTime(EpochTime time)
{
	this->m_start = time;
}

EpochTime CameraTrajectory::GetCurrentEpochTime(void) const
{
	return this->m_start.AddMilliSeconds(m_elapsedmsec);
}

EpochTime CameraTrajectory::GetStartEpochTime(void) const
{
	return this->m_start;
}

unsigned CameraTrajectory::GetCurrentFrameNum(void) const
{
	return this->m_currentFrameNum;
}
unsigned CameraTrajectory::GetNumberOfFrame(void) const
{
	return static_cast<unsigned>(this->m_elapsedmsecs.size());
}
void CameraTrajectory::SetCurrentFrame(unsigned frameNum)
{
	this->m_elapsedmsec = this->m_elapsedmsecs.at(frameNum);
}

IRenderableState CameraTrajectory::GetTrajectoryPolyLineState() const
{
	THROW_TREXCEPTION_IF_FAILED((m_polyLine != nullptr), L"Null pointer exception");
	return m_polyLine->GetState();
}

void CameraTrajectory::SetSpeed(float speed)
{
	THROW_TREXCEPTION_IF_FAILED((speed >= 0.0f), L"Failed to set speed a negative number");
	m_speed = speed;
}

float CameraTrajectory::GetSpeed(void) const
{
	return m_speed;
}



