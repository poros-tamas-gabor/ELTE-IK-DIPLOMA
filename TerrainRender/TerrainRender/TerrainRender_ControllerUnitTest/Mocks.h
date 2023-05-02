#pragma once
#include "pch.h"

class MockController : public IController {
public:
    MOCK_METHOD(bool, HandleMessage, (IControllerMessageIDs message, const std::vector<float>& fparam, const std::vector<unsigned>& uparam), (override));
    MOCK_METHOD(void, SetTerrainModel, (IModelPtr pModel), (override));
    MOCK_METHOD(void, SetTerrainView, (IViewPtr pView), (override));
    MOCK_METHOD(void, SetMouse, (MousePtr mouse), (override));
    MOCK_METHOD(void, SetKeyboard, (KeyboardPtr keyboard), (override));
    MOCK_METHOD(bool, Initialize, (IModelPtr pModel, IViewPtr pView, MousePtr mouse, KeyboardPtr keyboard), (override));
    MOCK_METHOD(void, Shutdown, (), (override));
    MOCK_METHOD(bool, IsActive, (), (const override));
    MOCK_METHOD(void, SetMessageSystem, (ControllerMessageSystemPtr messageSystem), (override));
    MOCK_METHOD(void, HandleIModelState, (const MeshGroupState&), (override));
    MOCK_METHOD(void, HandleIModelState, (const FlythroughState&), (override));
    MOCK_METHOD(void, HandleIModelState, (const Explore3DState&), (override));
    MOCK_METHOD(void, HandleIModelState, (const GeneralModelState&), (override));
};

class MockIModel : public IModel
{
public:
    MOCK_METHOD(bool, Initalize, (IDataAccessPtr persistence, Microsoft::WRL::ComPtr<ID3D11Device> device, int screenWidth, int screenHeight, float screenNear, float screenDepth, float fieldOfView), (override));
    MOCK_METHOD(bool, Resize, (unsigned screenWidth, unsigned screenHeight), (override));
    MOCK_METHOD(void, Shutdown, (), (override));
    MOCK_METHOD(bool, Render, (Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext), (override));
    MOCK_METHOD(bool, HandleMessage, (IModelMessageIDs messageID, const std::vector<std::wstring>& stringParams, const std::vector<float>& fparams, const std::vector<unsigned>& uparams), (override));
    MOCK_METHOD(bool, IsTrajectoryInitialized, (), (const override));
};

class MockIView : public IView
{
public:
    MOCK_METHOD(bool, Initalize, (HWND hwnd, float screenWidth, float screenHeight, bool fullscreen, bool vsync), (override));
    MOCK_METHOD(bool, RenderFrame, (), (override));
    MOCK_METHOD(bool, Resize, (unsigned screenWidth, unsigned screenHeight), (override));
    MOCK_METHOD(bool, HandleMessage, (IViewMessageIDs messageID, const std::vector<std::wstring>& stringParams, const std::vector<float>& fparams, const std::vector<unsigned>& uparams), (override));
    MOCK_METHOD(void, Shutdown, (), (override));
    MOCK_METHOD(void, SetController, (IControllerPtr terrainController), (override));
    MOCK_METHOD(void, SetModel, (IModelPtr terrainModel), (override));
    MOCK_METHOD(Microsoft::WRL::ComPtr<ID3D11Device>, GetDevice, (), (override));
    MOCK_METHOD(Microsoft::WRL::ComPtr<ID3D11DeviceContext>, GetDeviceContext, (), (override));

    MOCK_METHOD(void, HandleIModelState, (const MeshGroupState&), (override));
    MOCK_METHOD(void, HandleIModelState, (const FlythroughState&), (override));
    MOCK_METHOD(void, HandleIModelState, (const Explore3DState&), (override));
    MOCK_METHOD(void, HandleIModelState, (const GeneralModelState&), (override));
};

class MockKeyboard : public Keyboard
{
public:
    MOCK_METHOD(bool, KeyIsPressed, (const unsigned char keycode), (const));
    MOCK_METHOD(KeyboardEvent, ReadKey, (), ());
    MOCK_METHOD(unsigned char, ReadChar, (), ());
    MOCK_METHOD(bool, KeyBufferIsEmpty, (), (const));
    MOCK_METHOD(bool, CharBufferIsEmpty, (), (const));
    MOCK_METHOD(void, ClearKeyStates, (), ());
    MOCK_METHOD(void, ClearKeyBuffer, (), ());
    MOCK_METHOD(void, ClearCharBuffer, (), ());
    MOCK_METHOD(void, OnKeyPressed, (const unsigned char key), ());
    MOCK_METHOD(void, OnKeyReleased, (const unsigned char key), ());
    MOCK_METHOD(void, OnChar, (const unsigned char key), ());
    MOCK_METHOD(void, EnableAutoRepeat, (), ());
    MOCK_METHOD(void, DisableAutoRepeat, (), ());
    MOCK_METHOD(bool, IsAutoRepeatEnabled, (), (const));
};

class MockMouse : public Mouse
{
public:
    MOCK_METHOD(void, OnLeftPressed, (int x, int y), ());
    MOCK_METHOD(void, OnLeftReleased, (int x, int y), ());
    MOCK_METHOD(void, OnRightPressed, (int x, int y), ());
    MOCK_METHOD(void, OnRightReleased, (int x, int y), ());
    MOCK_METHOD(void, OnMiddlePressed, (int x, int y), ());
    MOCK_METHOD(void, OnMiddleReleased, (int x, int y), ());
    MOCK_METHOD(void, OnWheelUp, (int x, int y), ());
    MOCK_METHOD(void, OnWheelDown, (int x, int y), ());
    MOCK_METHOD(void, OnMouseMove, (int x, int y), ());
    MOCK_METHOD(void, OnMouseMoveRawRelative, (int x, int y), ());
    MOCK_METHOD(void, OnMouseMoveRawAbsolute, (int x, int y), ());
    MOCK_METHOD(bool, IsLeftDown, (), (const));
    MOCK_METHOD(bool, IsMiddleDown, (), (const));
    MOCK_METHOD(bool, IsRightDown, (), (const));
    MOCK_METHOD(int, GetPosX, (), (const));
    MOCK_METHOD(int, GetPosY, (), (const));
    MOCK_METHOD(MousePoint, GetPos, (), (const));
    MOCK_METHOD(bool, EventBufferIsEmpty, (), (const));
    MOCK_METHOD(MouseEvent, ReadEvent, (), ());
    MOCK_METHOD(void, ClearMouseEventBuffer, (), ());
    MOCK_METHOD(void, ClearMouseState, (), ());
};



