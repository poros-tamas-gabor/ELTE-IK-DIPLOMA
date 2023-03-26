#include "D3DView.h"
#include "../ErrorHandler.h"


D3DView::D3DView() : 
	_swapChain(nullptr),
	_device(nullptr),
	_deviceContext(nullptr),
	_renderTargetView(nullptr),
	
	_depthStencilBuffer(nullptr),
	_depthStencilView(nullptr),
	_depthStencilState(nullptr),
	_rasterState(nullptr),
	_videoCardDescription(""),
	_videoCardMemory(NULL)
{}

D3DView::~D3DView() {}


bool D3DView::GetAdapterData(float screenWidth, float screenHeight, unsigned int& numerator, unsigned int& denominator)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes = 0;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	unsigned int stringLength;

	try
	{
		// Create a DirectX graphics interface factory.
		result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);

		THROW_COM_EXCEPTION_IF_FAILED(result, L"Failed to Create DXGI Factory for enumerating Adapters");

		// Use the factory to create an adapter for the primary graphics interface (video card).
		result = factory->EnumAdapters(0, &adapter);

		THROW_COM_EXCEPTION_IF_FAILED(result, L"Failed to create adapter for the primary graphics interface ");

		// Enumerate the primary adapter output (monitor).
		result = adapter->EnumOutputs(0, &adapterOutput);

		THROW_COM_EXCEPTION_IF_FAILED(result, L"Failed to Enumerate the primary adapter output");

		// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);

		THROW_COM_EXCEPTION_IF_FAILED(result, L"Failed to Get the number of mode");

		// Create a list to hold all the possible display modes for this monitor/video card combination.
		displayModeList = new DXGI_MODE_DESC[numModes];
		if (!displayModeList)
		{
			throw TRException(L"Null pointer exception", __FILEW__, __FUNCTIONW__, __LINE__);
		}

		// Now fill the display mode list structures.
		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);

		THROW_COM_EXCEPTION_IF_FAILED(result, L"Failed to fill the display mode list structure");

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

		THROW_COM_EXCEPTION_IF_FAILED(result, L"Failed to get adapter description");

		// Store the dedicated video card memory in megabytes.
		this->_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		// Convert the name of the video card to a character array and store it.
		int error = wcstombs_s(&stringLength, this->_videoCardDescription, 128, adapterDesc.Description, 128);
		if (error != 0)
		{
			throw TRException(L"wcstombs_s exception", __FILEW__, __FUNCTIONW__, __LINE__);
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
	}
	catch (TRException& exception)
	{
		ErrorHandler::Log(exception);
		return false;
	}
	catch (COMException& exception)
	{
		ErrorHandler::Log(exception);
		return false;
	}

	return true;
}


bool D3DView::Initalize(HWND hwnd, float screenWidth, float screenHeight, bool fullscreen, bool vsync)
{
	try {
		HRESULT result;
		D3D_FEATURE_LEVEL featureLevel;
		ID3D11Texture2D* backBufferPtr;
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
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

		// Initialize the swap chain description.
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		swapChainDesc.BufferCount = 1;										// Set to a single back buffer.
		swapChainDesc.BufferDesc.Width = screenWidth;						// Set the width and height of the back buffer.
		swapChainDesc.BufferDesc.Height = screenHeight;						// Set the width and height of the back buffer.
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// Set regular 32-bit surface for the back buffer.

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

		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// Set the usage of the back buffer.
		swapChainDesc.OutputWindow = hwnd;								// Set the handle for the window to render to.
		swapChainDesc.SampleDesc.Count = 1;								// Turn multisampling off.
		swapChainDesc.SampleDesc.Quality = 0;							// Turn multisampling off

		// Set to full screen or windowed mode.
		if (fullscreen)
		{
			swapChainDesc.Windowed = false;
		}
		else
		{
			swapChainDesc.Windowed = true;
		}

		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; 	// Set the scan line ordering and scaling to unspecified.
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;								// Discard the back buffer contents after presenting.
		swapChainDesc.Flags = 0;															// Don't set the advanced flags.


		featureLevel = D3D_FEATURE_LEVEL_11_0;// Set the feature level to DirectX 11.

		// Create the swap chain, Direct3D device, and Direct3D device context.

		result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
			D3D11_SDK_VERSION, &swapChainDesc, &this->_swapChain, &this->_device, NULL, &this->_deviceContext);

		THROW_COM_EXCEPTION_IF_FAILED(result, L"Failed to Create the swap chain, Direct3D device, and Direct3D device context");

		// Get the pointer to the back buffer.
		result = this->_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);

		THROW_COM_EXCEPTION_IF_FAILED(result, L"Failed to Get the pointer to the back buffer");

		// Create the render target view with the back buffer pointer.
		result = this->_device->CreateRenderTargetView(backBufferPtr, NULL, &this->_renderTargetView);

		THROW_COM_EXCEPTION_IF_FAILED(result, L"Failed to Create the render target view with the back buffer pointer");

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

		THROW_COM_EXCEPTION_IF_FAILED(result, L"Failed to Create the texture for the depth buffer");

		//
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

		THROW_COM_EXCEPTION_IF_FAILED(result, L"Failed to Create the depth stencil state");

		// Set the depth stencil state.
		this->_deviceContext->OMSetDepthStencilState(this->_depthStencilState, 1);
		
		// Initialize the depth stencil view.
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
		
		// Set up the depth stencil view description.
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		
		//Create the depth stencil view.
		result = this->_device->CreateDepthStencilView(this->_depthStencilBuffer, &depthStencilViewDesc, &this->_depthStencilView);

		THROW_COM_EXCEPTION_IF_FAILED(result, L"Failed to Create the depth stencil view");

		// Bind the render target view and depth stencil buffer to the output render pipeline.
		this->_deviceContext->OMSetRenderTargets(1, &this->_renderTargetView, this->_depthStencilView);


		// Setup the raster description which will determine how and what polygons will be drawn.
		ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));

		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;//D3D11_CULL_NONE;//D3D11_CULL_BACK;
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

		THROW_COM_EXCEPTION_IF_FAILED(result, L"Failed to Create the rasterizer state from the description we just filled out.");

		// Now set the rasterizer state.
		this->_deviceContext->RSSetState(this->_rasterState);

		// Setup the viewport for rendering.
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.Width = (float)screenWidth;
		viewport.Height = (float)screenHeight;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		//
		// Create the viewport.
		this->_deviceContext->RSSetViewports(1, &viewport);
	}

	catch (COMException& exception)
	{
		ErrorHandler::Log(exception);
		return false;
	}
	return true;

}
void D3DView::Shutdown() {

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
void D3DView::BeginScene(float red, float green, float blue, float alpha) {

	float color[] = { red, green, blue, alpha };
	// clear the back buffer to the color
	_deviceContext->ClearRenderTargetView(_renderTargetView, color);

	// Clear the depth buffer.
	_deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

}
void D3DView::EndScene() {
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

ID3D11Device* D3DView::GetDevice()
{
	return this->_device;
}

ID3D11DeviceContext* D3DView::GetDeviceContext()
{
	return this->_deviceContext;
}
