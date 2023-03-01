#pragma once
#ifndef	D3D_MANAGER_H
#define	D3D_MANAGER_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")


#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>




class D3DManager
{
private:
	IDXGISwapChain*			_swapChain;
	ID3D11Device*			_device;
	ID3D11DeviceContext*	_deviceContext;
	ID3D11RenderTargetView* _renderTargetView;

	ID3D11Texture2D*		_depthStencilBuffer;
	ID3D11DepthStencilView* _depthStencilView;
	ID3D11DepthStencilState* _depthStencilState;
	ID3D11RasterizerState*	_rasterState;

	//D3DXMATRIX				_projectionMatrix;
	//D3DXMATRIX				_worldMatrix;
	//D3DXMATRIX				_orthoMatrix;

	bool					_vsync = true;
	int						_videoCardMemory;
	char					_videoCardDescription[128];

	bool GetAdapterData(float screenWidth, float screenHeight, unsigned int& numerator, unsigned int& denominator);
public:
	D3DManager();
	~D3DManager();
	bool Initalize(HWND hwnd, float screenWidth, float screenHeight, float screenNear, float screenDepth, bool fullscree, bool vsync, float fieldOfView);
	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();


	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	//void GetProjectionMatrix(D3DXMATRIX& projectionMatrix);
	//void GetWorldMatrix(D3DXMATRIX& worldMatrix);
	//void GetOrthoMatrix(D3DXMATRIX& orthoMatrix);




};
#endif