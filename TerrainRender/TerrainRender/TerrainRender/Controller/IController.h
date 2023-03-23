#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include "../Input/Mouse.h"
#include "../Input/Keyboard.h"
#include <memory>

class MessageSystem;
class TerrainModel; //TODO: Use IModel
class IController {
public:
    virtual ~IController() {}
    virtual bool CanHandle(unsigned int) const = 0;
    virtual void HandleMessage(unsigned int message, float* fparam, unsigned* uparam) = 0;

    virtual void SetTerrainModel(TerrainModel* pModel) = 0;
    virtual void SetMouse(Mouse* mouse) = 0;
    virtual void SetKeyboard(Keyboard* keyboard) = 0;

    virtual bool Initialize(TerrainModel* pModel, Mouse* mouse, Keyboard* keyboard) = 0;

    virtual void Shutdown() = 0;
    virtual bool IsActive() const = 0 ;
    virtual void SetMessageSystem(MessageSystem* messageSystem) = 0;
};
typedef std::shared_ptr<IController> IControllerPtr;




#endif
