#pragma once
#include "pch.h"

//class MockController : public IController {
//public:
//    MOCK_METHOD(bool, HandleMessage, (IControllerMessageIDs message, const std::vector<float>& fparam, const std::vector<unsigned>& uparam), (override));
//    MOCK_METHOD(void, SetTerrainModel, (IModelPtr pModel), (override));
//    MOCK_METHOD(void, SetTerrainView, (IViewPtr pView), (override));
//    MOCK_METHOD(void, SetMouse, (MousePtr mouse), (override));
//    MOCK_METHOD(void, SetKeyboard, (KeyboardPtr keyboard), (override));
//    MOCK_METHOD(bool, Initialize, (IModelPtr pModel, IViewPtr pView, MousePtr mouse, KeyboardPtr keyboard), (override));
//    MOCK_METHOD(void, Shutdown, (), (override));
//    MOCK_METHOD(bool, IsActive, (), (const override));
//    MOCK_METHOD(void, SetMessageSystem, (ControllerMessageSystemPtr messageSystem), (override));
//    MOCK_METHOD(void, HandleIModelState, (const MeshGroupState&), (override));
//    MOCK_METHOD(void, HandleIModelState, (const FlythroughState&), (override));
//    MOCK_METHOD(void, HandleIModelState, (const Explore3DState&), (override));
//    MOCK_METHOD(void, HandleIModelState, (const GeneralModelState&), (override));
//};