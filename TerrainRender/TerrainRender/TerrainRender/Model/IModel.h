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

	virtual bool	Initalize(IDataAccessPtr persistence, Microsoft::WRL::ComPtr<ID3D11Device> device, int screenWidth, int screenHeight, float screenNear, float screenDepth, float fieldOfView) = 0;
	virtual bool	Resize(unsigned screenWidth, unsigned screenHeight) = 0;
	virtual void	Shutdown() = 0;
	virtual bool	Render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext) = 0;

	virtual bool	HandleMessage(IModelMessageIDs messageID, const std::vector<std::wstring>& stringParams, const std::vector<float>& fparams, const std::vector<unsigned>& uparams) = 0;

	virtual bool	IsTrajectoryInitialized(void) const = 0;
};
typedef std::shared_ptr<IModel> IModelPtr;




#endif