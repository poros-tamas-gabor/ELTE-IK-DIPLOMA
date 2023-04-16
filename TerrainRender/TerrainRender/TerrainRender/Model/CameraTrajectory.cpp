#include "CameraTrajectory.h"
#include "../ErrorHandler.h"

bool CameraTrajectory::Initialize(const std::vector<CameraPose>& cameraPoses, IRendarablePtr<VertexPolyLine> renderable, Camera* camera)
{
	if (renderable == nullptr || camera == nullptr)
	{
		return false;
	}
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



void CameraTrajectory::Reset()
{
	this->m_elapsedmsec = 0;
	this->m_camera->SetPosition(m_positions.at(0).x, m_positions.at(0).y, m_positions.at(0).z);
	this->m_camera->SetRotationRad(m_rotations.at(0).x, m_rotations.at(0).y, m_rotations.at(0).z);

}

Vector3D  CameraTrajectory::TransformPosition(const Vector3D& vector) const
{
	DirectX::XMMATRIX rotation = m_polyLine->GetWorldMatrix();
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(vector.x, vector.y, vector.z, 1.0f);
	vec = DirectX::XMVector4Transform(vec, rotation);
	DirectX::XMFLOAT3 vecfloat3;
	DirectX::XMStoreFloat3(&vecfloat3, vec);
	return { vecfloat3.x,vecfloat3.y,vecfloat3.z };

}
Vector3D  CameraTrajectory::TransformRotation(const Vector3D& vector) const
{
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

void CameraTrajectory::UpdateCamera(double elapsedmsec)
{
	bool result;
	Vector3D currentCameraRotation;
	Vector3D currentCameraPosition;

	this->m_elapsedmsec += elapsedmsec;

	LinearInterpolation<double, Vector3D>	linearInterpolation;
	result = linearInterpolation.Calculate(this->m_elapsedmsecs, this->m_positions, this->m_elapsedmsec, currentCameraPosition, m_currentFrameNum);
	if (!result)
		return;

	CirclularInterpolation<double> circularInterpolation;
	result = circularInterpolation.Calculate(this->m_elapsedmsecs, this->m_rotations, this->m_elapsedmsec, currentCameraRotation, m_currentFrameNum);
	if (!result)
		return;

	currentCameraRotation = TransformRotation(currentCameraRotation);
	currentCameraPosition = TransformPosition(currentCameraPosition);
	
	this->m_camera->SetPosition(currentCameraPosition.x, currentCameraPosition.y, currentCameraPosition.z);
	this->m_camera->SetRotationRad(currentCameraRotation.x, currentCameraRotation.y, currentCameraRotation.z);
}

EpochTime CameraTrajectory::GetCurrentEpochTime(void) const
{
	return this->m_start.AddMilliSeconds(m_elapsedmsec);
}

unsigned CameraTrajectory::GetCurrentFrameNum(void) const
{
	return this->m_currentFrameNum;
}
unsigned CameraTrajectory::GetNumberOfFrame(void) const
{
	return this->m_elapsedmsecs.size();
}
void CameraTrajectory::SetCurrentFrame(unsigned frameNum)
{
	try
	{
		this->m_elapsedmsec = this->m_elapsedmsecs.at(frameNum);
	}
	catch (std::exception& e)
	{
		ErrorHandler::Log(e);
	}
}

IRenderableState CameraTrajectory::GetTrajectoryPolyLineState() const
{
	return m_polyLine->GetState();
}

void CameraTrajectory::Rotate(Vector3D rotations)
{
	if (IsInitialized())
	{
		m_polyLine->Rotate(rotations.x, rotations.y, rotations.z);
	}
}
void CameraTrajectory::Move(Vector3D   translationVector)
{
	if (IsInitialized())
	{
		m_polyLine->Translate(translationVector.x, translationVector.y, translationVector.z);
	}
}


