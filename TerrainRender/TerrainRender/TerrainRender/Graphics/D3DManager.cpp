#include "D3DManager.h"
#include "../ErrorHandler.h"

D3DManager::D3DManager() : 
	_swapChain(nullptr),
	_device(nullptr),
	_deviceContext(nullptr),
	_renderTargetView(nullptr),
	
	_depthStencilBuffer(nullptr),
	_depthStencilView(nullptr),
	_depthStencilState(nullptr),
	_rasterState(nullptr) {}

D3DManager::~D3DManager() {}


bool D3DManager::GetAdapterData(float screenWidth, float screenHeight, unsigned int& numerator, unsigned int& denominator)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	unsigned int stringLength;


	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		ErrorHandler::log(result, L"Failed to Create DXGI Factory for enumerating Adapters");
		return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		ErrorHandler::log(result, L"Failed to create adapter for the primary graphics interface ");
		return false;
	}

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		ErrorHandler::log(result, L"Failed to Enumerate the primary adapter output");
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		ErrorHandler::log(result, L"Failed to Get the number of mode");
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		ErrorHandler::log(result, L"Failed to fill the display mode list structure");
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		ErrorHandler::log(result, L"Failed to get adapter description");
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	this->_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	int error = wcstombs_s(&stringLength, this->_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	// Release the display mode list.
	delete[] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;

	return true;
}


bool D3DManager::Initalize(HWND hwnd, float screenWidth, float screenHeight, float screenNear, float screenDepth, bool fullscreen, bool vsync, float fieldOfView)
{
	HRESULT result;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;




	// Store the vsync setting.
	this->_vsync = vsync;

	unsigned int numerator, denominator;
	if (!this->GetAdapterData(screenWidth, screenHeight, numerator, denominator))
	{
		return false;
	}
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if (this->_vsync)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = hwnd;

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &this->_swapChain, &this->_device, NULL, &this->_deviceContext);
	if (FAILED(result))
	{
		ErrorHandler::log(result, L"Failed to Create the swap chain, Direct3D device, and Direct3D device context");
		return false;
	}

	// Get the pointer to the back buffer.
	result = this->_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		ErrorHandler::log(result, L"Failed toGet the pointer to the back buffer");
		return false;
	}

	// Create the render target view with the back buffer pointer.
	result = this->_device->CreateRenderTargetView(backBufferPtr, NULL, &this->_renderTargetView);
	if (FAILED(result))
	{
		ErrorHandler::log(result, L"Failed to Create the render target view with the back buffer pointer");
		return false;
	}

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	
	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	
	// Create the texture for the depth buffer using the filled out description.
	result = this->_device->CreateTexture2D(&depthBufferDesc, NULL, &this->_depthStencilBuffer);
	if (FAILED(result))
	{
		ErrorHandler::log(result, L"Failed to Create the texture for the depth buffer using the filled out description");
		return false;
	}
	
	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	
	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	
	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	
	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	
	// Create the depth stencil state.
	result = this->_device->CreateDepthStencilState(&depthStencilDesc, &this->_depthStencilState);
	if (FAILED(result))
	{
		ErrorHandler::log(result, L"Failed to Create the depth stencil state");
	
		return false;
	}
	
	// Set the depth stencil state.
	this->_deviceContext->OMSetDepthStencilState(this->_depthStencilState, 1);
	
	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	
	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	
	// Create the depth stencil view.
	result = this->_device->CreateDepthStencilView(this->_depthStencilBuffer, &depthStencilViewDesc, &this->_depthStencilView);
	if (FAILED(result))
	{
		ErrorHandler::log(result, L"Failed to Create the depth stencil view");
	
		return false;
	}
	
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	this->_deviceContext->OMSetRenderTargets(1, &this->_renderTargetView, this->_depthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;//D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	
	// Create the rasterizer state from the description we just filled out.
	result = this->_device->CreateRasterizerState(&rasterDesc, &this->_rasterState);
	if (FAILED(result))
	{
		ErrorHandler::log(result, L"Failed to Create the rasterizer state from the description we just filled out.");
		
		return false;
	}
	
	// Now set the rasterizer state.
	this->_deviceContext->RSSetState(this->_rasterState);

	// Setup the viewport for rendering.
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	this->_deviceContext->RSSetViewports(1, &viewport);

	// Setup the projection matrix.
	float screenAspect = (float)screenWidth / (float)screenHeight;

	//// Create the projection matrix for 3D rendering.
	//D3DXMatrixPerspectiveFovLH(&this->_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);
	//
	//// Initialize the world matrix to the identity matrix.
	//D3DXMatrixIdentity(&this->_worldMatrix);
	//
	//// Create an orthographic projection matrix for 2D rendering.
	//D3DXMatrixOrthoLH(&this->_orthoMatrix, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	return true;

}
void D3DManager::Shutdown() {

	if (this->_swapChain)
	{
		this->_swapChain->Release();
		this->_swapChain = nullptr;
	}

	if (this->_device)
	{
		this->_device->Release();
		this->_device = nullptr;
	}
	if (this->_deviceContext)
	{
		this->_deviceContext->Release();
		this->_deviceContext = nullptr;
	}
	if (this->_renderTargetView)
	{
		this->_renderTargetView->Release();
		this->_renderTargetView = nullptr;
	}
	if (this->_depthStencilBuffer)
	{
		this->_depthStencilBuffer->Release();
		this->_depthStencilBuffer = nullptr;
	}
	if (this->_depthStencilView)
	{
		this->_depthStencilView->Release();
		this->_depthStencilView = nullptr;
	}
	if (this->_depthStencilState)
	{
		this->_depthStencilState->Release();
		this->_depthStencilState = nullptr;
	}
	if (this->_rasterState)
	{
		this->_rasterState->Release();
		this->_rasterState = nullptr;
	}
}
void D3DManager::BeginScene(float red, float green, float blue, float alpha) {

	float color[] = { red, green, blue, alpha };
	// clear the back buffer to the color
	_deviceContext->ClearRenderTargetView(_renderTargetView, color);

	// Clear the depth buffer.
	//_deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

}
void D3DManager::EndScene() {
	// Present the back buffer to the screen since rendering is complete.
	if (this->_vsync)
	{
		// Lock to screen refresh rate.
		this->_swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		this->_swapChain->Present(0, 0);
	}

}


ID3D11Device* D3DManager::GetDevice()
{
	return this->_device;
}


ID3D11DeviceContext* D3DManager::GetDeviceContext()
{
	return this->_deviceContext;
}

//void D3DManager::GetProjectionMatrix(D3DXMATRIX& projectionMatrix)
//{
//	projectionMatrix = this->_projectionMatrix;
//}
//
//
//void D3DManager::GetWorldMatrix(D3DXMATRIX& worldMatrix)
//{
//	worldMatrix = this->_worldMatrix;
//}
//
//
//void D3DManager::GetOrthoMatrix(D3DXMATRIX& orthoMatrix)
//{
//	orthoMatrix = this->_orthoMatrix;
//}