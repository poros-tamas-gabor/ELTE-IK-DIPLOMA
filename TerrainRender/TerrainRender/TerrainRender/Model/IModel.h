#ifndef I_MODEL_H
#define I_MODEL_H
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <memory>
#include <vector>
#include <string>
#include <wrl/client.h>
#include "Persistence/DataAccess.h"
#include <d3d11.h>
class IModel
{
public:

	virtual ~IModel() = default;

	virtual bool	Initalize(HWND hwnd, IDataAccessPtr persistence, Microsoft::WRL::ComPtr<ID3D11Device> device, int screenWidth, int screenHeight, float screenNear, float screenDepth, float fieldOfView) = 0;
	virtual void	Resize(unsigned screenWidth, unsigned screenHeight) = 0;
	virtual void	Shutdown() = 0;
	virtual bool	Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext) = 0;

	virtual bool	LoadTerrainSharpEdges(const wchar_t* filepath) = 0;
	virtual bool	LoadTerrainSoftEdges(const wchar_t* filepath) = 0;
	virtual bool	LoadCameraTrajectory(const wchar_t* filepath) = 0;
	virtual bool	LoadParameters(const wchar_t* filepath) = 0;
	virtual bool	LoadTerrainSharpEdges_Project(const std::vector<std::wstring>& files) = 0;
	virtual bool	LoadTerrainSoftEdges_Project(const std::vector<std::wstring>& files) = 0;
 
	virtual bool	IsTrajectoryInitialized(void) const = 0;

	virtual void	HandleFlythroughMode(unsigned message, float* elapsedMillisec, unsigned* frameNum) = 0;
	virtual void	HandleExplore3DMode(unsigned message, float* fparams) = 0;


	//virtual void	MoveCamera(unsigned message, float timeElapsed) = 0;
	//virtual void	RotateCamera(unsigned message, float pitch, float yaw) = 0;
	virtual void	ResetCamera(void) = 0;
	virtual void	UpdateCameraProperties(unsigned message, float data) = 0;
	virtual void	TransformIRenderable(unsigned message, unsigned id, float parameters[]) = 0;
	virtual void	SetUnixTime(unsigned message, unsigned* uparam) = 0;
	virtual void	ClearTerrain(void) = 0;
	virtual void	ClearCameraTrajectory(void) = 0;
};

typedef std::shared_ptr<IModel> IModelPtr;


#endif