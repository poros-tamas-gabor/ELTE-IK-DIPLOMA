#ifndef	D3D_MANAGER_H
#define	D3D_MANAGER_H

///////////////////////////////////////////////////////////////////////////////
// D3DView.h
// ==========
// A class responsible for creating, managing and releasing DirectX objects necessary to use the DirectX API and communicate
// with the hardware through the low-level graphics API. The class also stores the size and resolution of the display surface,
// and is responsible for saving simulation images.
//
// The class contains methods to initialize DirectX objects and resources, set up the render pipeline, render the scene,
// handle changes in window size or display settings, and handle keyboard and mouse input.
//
// AUTHOR: Pindrought
// URL: https://github.com/Pindrought/DirectX-11-Engine-VS2017
// CREATED: Unknown
// MODIFIED: 2023-05-08 by TAMAS GABOR POROS
///////////////////////////////////////////////////////////////////////////////

#include "../win.h"
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <string>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "runtimeobject.lib")


class D3DView
{
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain>				m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>				m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			m_deviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		m_renderTargetView;

	Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		m_depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		m_depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		m_rasterState;

	HWND	m_hwnd;

	bool	m_vsync = true;
	int		m_videoCardMemory;
	char	m_videoCardDescription[128];

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
	void CaptureScreen(const std::wstring& directoryPath, unsigned frameNum);

	void BeginScene(float, float, float, float);
	void EndScene();

	Microsoft::WRL::ComPtr<ID3D11Device>		GetDevice();
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>	GetDeviceContext();
};
#endif