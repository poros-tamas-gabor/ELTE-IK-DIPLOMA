#ifndef I_MODEL_H
#define I_MODEL_H

///////////////////////////////////////////////////////////////////////////////
// IModel.h
// ========
// An interface for the Model component in the Model-View-Controller (MVC) architecture.
//
// The purpose of the Model in MVC is to manage the data and logic of the application.
// The Model receives commands from the Controller and updates the data accordingly, and can also notify the View of any changes.
//
// This is only an interface and does not contain any implementation details. Programmers must implement a derived class to handle
// specific data and logic for their application.
//
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

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