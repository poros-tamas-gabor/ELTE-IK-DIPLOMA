#ifndef I_VIEW_H
#define I_VIEW_H

///////////////////////////////////////////////////////////////////////////////
// IView.h
// =======
// An interface for the View component in the Model-View-Controller (MVC) architecture.
//
// The purpose of the View in MVC is to display the Model's data to the user and receive input from the user.
// The View is responsible for creating and managing the user interface elements, such as buttons and text fields,
// and updating them based on changes in the Model's data.
//
// This is only an interface and does not handle any messages. Programmers must implement a derived class to handle
// specific messages.
//
// AUTHOR: TAMAS GABOR POROS
// CREATED: 2023-05-08
///////////////////////////////////////////////////////////////////////////////

#include <memory>
#include <vector>
#include "../Model/Persistence/ModelStructs.h"
#include "../Model/IModelSubscriber.h"
#include <string>

class IController;
typedef std::shared_ptr<IController> IControllerPtr;

class IView : public IModelSubscriber
{

public:
	virtual ~IView() {}

	virtual bool Initalize(HWND hwnd, float screenWidth, float screenHeight, bool fullscreen, bool vsync) = 0;
	virtual bool RenderFrame() = 0;
	virtual	bool Resize(unsigned screenWidth, unsigned screenHeight) = 0;
	virtual void Shutdown() = 0;

	virtual bool HandleMessage(IViewMessageIDs message, const std::vector<std::wstring>& stringParams, const std::vector<float>& fparams, const std::vector<unsigned>& uparams) = 0;

	virtual void SetController(IControllerPtr terrainController) = 0;
	virtual void SetModel(IModelPtr terrainModel) = 0;

	virtual Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() = 0;
	virtual Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetDeviceContext() = 0;
};

typedef std::shared_ptr<IView> IViewPtr;
#endif