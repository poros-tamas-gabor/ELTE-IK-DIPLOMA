#ifndef I_MODEL_H
#define I_MODEL_H
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <memory>
#include <vector>
#include <string>
#include <wrl/client.h>
#include "Persistence/DataAccess.h"
#include "../resource.h"
#include <d3d11.h>
class IModel
{
public:

	virtual ~IModel() = default;

	virtual bool	Initalize(HWND hwnd, IDataAccessPtr persistence, Microsoft::WRL::ComPtr<ID3D11Device> device, int screenWidth, int screenHeight, float screenNear, float screenDepth, float fieldOfView) = 0;
	virtual void	Resize(unsigned screenWidth, unsigned screenHeight) = 0;
	virtual void	Shutdown() = 0;
	virtual bool	Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext) = 0;

	virtual bool	HandleMessage(IModelMessageIDs messageID, const std::vector<std::wstring>& stringParams, const std::vector<float>& fparams, const std::vector<unsigned>& uparams) = 0;

	virtual bool	IsTrajectoryInitialized(void) const = 0;

	//virtual bool	LoadTerrain_withSharpEdges(const wchar_t* filepath) = 0;
	//virtual bool	LoadTerrain_withSoftEdges(const wchar_t* filepath) = 0;
	//virtual bool	LoadCameraTrajectory(const wchar_t* filepath) = 0;
	//virtual bool	LoadConfigurationFile(const wchar_t* filepath) = 0;
	//virtual bool	LoadProject_withSharpEdges(const std::vector<std::wstring>& files) = 0;
	//virtual bool	LoadProject_withSoftEdges(const std::vector<std::wstring>& files) = 0;
 	//
	//
	//virtual void	HandleFlythroughMode(unsigned message, float* elapsedMillisec, unsigned* frameNum) = 0;
	//virtual void	HandleExplore3DMode(unsigned message, float* fparams) = 0;
	//
	//
	////virtual void	MoveCamera(unsigned message, float timeElapsed) = 0;
	////virtual void	RotateCamera(unsigned message, float pitch, float yaw) = 0;
	//virtual void	ResetCamera(void) = 0;
	//virtual void	SetCameraProperties(unsigned message, float data) = 0;
	//virtual void	TransformMeshElement(unsigned message, unsigned id, float parameters[]) = 0;
	//virtual void	SetUnixTime(unsigned message, unsigned* uparam) = 0;
	//virtual void	ClearMeshes(void) = 0;
	//virtual void	ClearCameraTrajectory(void) = 0;
};
typedef std::shared_ptr<IModel> IModelPtr;




#endif