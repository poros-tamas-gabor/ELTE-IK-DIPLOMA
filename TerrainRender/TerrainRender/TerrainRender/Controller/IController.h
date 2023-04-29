#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include <memory>
#include "../Model/IModelSubscriber.h"
#include "../Model/IModel.h"
#include "../resource.h"
#include <vector>

class ControllerMessageSystem;
typedef std::shared_ptr<ControllerMessageSystem> ControllerMessageSystemPtr;
class IView;
typedef std::shared_ptr<IView> IViewPtr;
class Mouse;
typedef std::shared_ptr<Mouse> MousePtr;
class Keyboard;
typedef std::shared_ptr<Keyboard> KeyboardPtr;

class IController : public IModelSubscriber  {
public:
    virtual ~IController() {}
    //virtual bool CanHandle(IControllerMessageIDs message) const = 0;
    //TODO const float* param, const unsigned* uparam
    virtual void HandleMessage(IControllerMessageIDs message, const std::vector<float>& fparam, const std::vector<unsigned>& uparam) = 0;

    virtual void SetTerrainModel(IModelPtr pModel) = 0;
    virtual void SetTerrainView(IViewPtr pView) = 0;
    virtual void SetMouse(MousePtr mouse) = 0;
    virtual void SetKeyboard(KeyboardPtr keyboard) = 0;

    virtual bool Initialize(IModelPtr pModel, IViewPtr pView, MousePtr mouse, KeyboardPtr keyboard) = 0;
    virtual bool IsFlythroughModeOn(void) const = 0;

    virtual void Shutdown() = 0;
    virtual bool IsActive() const = 0 ;
    virtual void SetMessageSystem(ControllerMessageSystemPtr messageSystem) = 0;
};
typedef class std::shared_ptr<IController> IControllerPtr;




#endif
