#ifndef I_MODEL_H
#define I_MODEL_H
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <memory>
#include <vector>
#include <string>

class	IDataAccess;
class  ID3D11Device;
class  ID3D11DeviceContext;
class IModel
{
public:

	virtual ~IModel() = default;

	virtual bool	Initalize(HWND hwnd, IDataAccess* persistence, ID3D11Device* device, int screenWidth, int screenHeight, float screenNear, float screenDepth, float fieldOfView) = 0;
	virtual void	Shutdown() = 0;
	virtual bool	Render(ID3D11DeviceContext* deviceContext) = 0;

	virtual bool	LoadTerrain(const wchar_t* filepath) = 0;
	virtual bool	LoadCameraTrajectory(const wchar_t* filepath) = 0;
	virtual bool	LoadTerrainProject(const std::vector<std::wstring>& files) = 0;
 
	virtual bool	IsTrajectoryLoaded(void) const = 0;

	virtual void	Flythrough(unsigned message, float* elapsedMillisec, unsigned* frameNum) = 0;
	virtual void	MoveCamera(unsigned message, float timeElapsed) = 0;
	virtual void	RotateCamera(unsigned message, float pitch, float yaw) = 0;
	virtual void	ResetCamera(void) = 0;
	virtual void	UpdateCameraProperties(unsigned message, float data) = 0;
	virtual void	TransformIRenderable(unsigned message, unsigned id, float parameters[]) = 0;
	virtual void	ClearTerrain(void) = 0;
	virtual void	ClearCameraTrajectory(void) = 0;
};

typedef std::shared_ptr<IModel> IModelPtr;


#endif