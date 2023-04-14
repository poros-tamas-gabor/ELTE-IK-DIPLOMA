#pragma once
#ifndef	D3D_MANAGER_H
#define	D3D_MANAGER_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "runtimeobject.lib")


#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <wrl/client.h>




class D3DView
{
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain>				_swapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>				_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			_deviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		_renderTargetView;

	Microsoft::WRL::ComPtr<ID3D11Texture2D>				_depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		_depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		_depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		_rasterState;

	HWND						_hwnd;

	bool						_vsync = true;
	int							_videoCardMemory;
	char						_videoCardDescription[128];

	bool GetAdapterData(float screenWidth, float screenHeight, unsigned int& numerator, unsigned int& denominator);
	bool InitalizeAttributes(unsigned screenWidth, unsigned screenHeight);
	bool ReinitalizeAttributes(unsigned screenWidth, unsigned screenHeight);
	HRESULT ReleaseBackBuffer();
public:
	D3DView();
	~D3DView();
	bool Initalize(HWND hwnd, float screenWidth, float screenHeight, bool fullscreen, bool vsync);
	bool Resize(unsigned screenWidth, unsigned screenHeight);
	void Shutdown();
	void CaptureScreen();

	void BeginScene(float, float, float, float);
	void EndScene();


	Microsoft::WRL::ComPtr<ID3D11Device>		GetDevice();
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>	GetDeviceContext();
};
#endif