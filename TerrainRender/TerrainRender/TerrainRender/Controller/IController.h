#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include "../Input/Mouse.h"
#include "../Input/Keyboard.h"
#include "../Model/IModel.h"
#include <memory>

class MessageSystem;
class IController {
public:
    virtual ~IController() {}
    virtual bool CanHandle(unsigned int) const = 0;
    //TODO const float* param, const unsigned* uparam
    virtual void HandleMessage(unsigned int message, float* fparam, unsigned* uparam) = 0;

    virtual void SetTerrainModel(IModelPtr pModel) = 0;
    virtual void SetMouse(Mouse* mouse) = 0;
    virtual void SetKeyboard(Keyboard* keyboard) = 0;

    virtual bool Initialize(IModelPtr pModel, Mouse* mouse, Keyboard* keyboard) = 0;

    virtual void Shutdown() = 0;
    virtual bool IsActive() const = 0 ;
    virtual void SetMessageSystem(MessageSystem* messageSystem) = 0;
};
typedef std::shared_ptr<IController> IControllerPtr;




#endif
