#ifndef MOCK_CLASSES_H
#define MOCK_CLASSES_H

#include "pch.h"

#include <d3d11.h>

// This class must mock QueryInterface, since a lot of things are
// QueryInterfac()ed thru this class.
/*class MockID3D11Device
    : public ID3D11Device {
public:
    MockID3D11Device() = default;
    ~MockID3D11Device() = default;

    // IUnknown methods
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject) {
        return E_NOINTERFACE;
    }
    STDMETHOD_(ULONG, AddRef)() {
        return 1;
    }
    STDMETHOD_(ULONG, Release)() {
        return 1;
    }

    MOCK_METHOD(HRESULT, CreateBuffer, (const D3D11_BUFFER_DESC*,
            const D3D11_SUBRESOURCE_DATA*,
            ID3D11Buffer**), (override));
    MOCK_METHOD(HRESULT, CreateTexture1D,
        (const D3D11_TEXTURE1D_DESC*,
            const D3D11_SUBRESOURCE_DATA*,
            ID3D11Texture1D**), (override));
    MOCK_METHOD(HRESULT,CreateTexture2D,
        (const D3D11_TEXTURE2D_DESC*,
            const D3D11_SUBRESOURCE_DATA*,
            ID3D11Texture2D**), (override));
    MOCK_METHOD(HRESULT,CreateTexture3D,
        (const D3D11_TEXTURE3D_DESC*,
            const D3D11_SUBRESOURCE_DATA*,
            ID3D11Texture3D**), (override));
    MOCK_METHOD(HRESULT,CreateShaderResourceView,
        (ID3D11Resource*,
            const D3D11_SHADER_RESOURCE_VIEW_DESC*,
            ID3D11ShaderResourceView**), (override));
    MOCK_METHOD(HRESULT,CreateUnorderedAccessView,
        (ID3D11Resource*,
            const D3D11_UNORDERED_ACCESS_VIEW_DESC*,
            ID3D11UnorderedAccessView**), (override));
    MOCK_METHOD(HRESULT,CreateRenderTargetView,
        (ID3D11Resource*,
            const D3D11_RENDER_TARGET_VIEW_DESC*,
            ID3D11RenderTargetView**), (override));
    MOCK_METHOD(HRESULT,CreateDepthStencilView,
        (ID3D11Resource*,
            const D3D11_DEPTH_STENCIL_VIEW_DESC*,
            ID3D11DepthStencilView**), (override));
    MOCK_METHOD(HRESULT, CreateInputLayout,
        (const D3D11_INPUT_ELEMENT_DESC*,
            UINT,
            const void*,
            SIZE_T,
            ID3D11InputLayout**), (override));
    MOCK_METHOD(HRESULT,
        CreateVertexShader,
        (const void*, SIZE_T, ID3D11ClassLinkage*, ID3D11VertexShader**), (override));
    MOCK_METHOD(HRESULT,CreateGeometryShader,
        (const void*,
            SIZE_T,
            ID3D11ClassLinkage*,
            ID3D11GeometryShader**), (override));
    MOCK_METHOD(HRESULT,CreateGeometryShaderWithStreamOutput,
        (const void*,
            SIZE_T,
            const D3D11_SO_DECLARATION_ENTRY*,
            UINT,
            const UINT*,
            UINT,
            UINT,
            ID3D11ClassLinkage*,
            ID3D11GeometryShader**), (override));
    MOCK_METHOD(HRESULT,
        CreatePixelShader,
        (const void*, SIZE_T, ID3D11ClassLinkage*, ID3D11PixelShader**), (override));
    MOCK_METHOD(HRESULT,
        CreateHullShader,
        (const void*, SIZE_T, ID3D11ClassLinkage*, ID3D11HullShader**), (override));
    MOCK_METHOD(HRESULT,
        CreateDomainShader,
        (const void*, SIZE_T, ID3D11ClassLinkage*, ID3D11DomainShader**), (override));
    MOCK_METHOD(HRESULT,
        CreateComputeShader,
        (const void*, SIZE_T, ID3D11ClassLinkage*, ID3D11ComputeShader**), (override));
    MOCK_METHOD(HRESULT,CreateClassLinkage, (ID3D11ClassLinkage**), (override));
    MOCK_METHOD(HRESULT,CreateBlendState,
        (const D3D11_BLEND_DESC*, ID3D11BlendState**), (override));
    MOCK_METHOD(HRESULT,CreateDepthStencilState,
        (const D3D11_DEPTH_STENCIL_DESC*,
            ID3D11DepthStencilState**), (override));
    MOCK_METHOD(HRESULT,CreateRasterizerState,
        (const D3D11_RASTERIZER_DESC*,
            ID3D11RasterizerState**), (override));
    MOCK_METHOD(HRESULT,CreateSamplerState,
        (const D3D11_SAMPLER_DESC*,
            ID3D11SamplerState**), (override));
    MOCK_METHOD(HRESULT,CreateQuery,
        (const D3D11_QUERY_DESC*, ID3D11Query**), (override));
    MOCK_METHOD(HRESULT,CreatePredicate,
        (const D3D11_QUERY_DESC*, ID3D11Predicate**), (override));
    MOCK_METHOD(HRESULT,CreateCounter,
        (const D3D11_COUNTER_DESC*, ID3D11Counter**), (override));
    MOCK_METHOD(HRESULT,CreateDeferredContext,
        (UINT, ID3D11DeviceContext**), (override));
    MOCK_METHOD(HRESULT,OpenSharedResource, (HANDLE, const IID&, void**), (override));
    MOCK_METHOD(HRESULT,CheckFormatSupport, (DXGI_FORMAT, UINT*));
    MOCK_METHOD(HRESULT,CheckMultisampleQualityLevels,
        (DXGI_FORMAT, UINT, UINT*));
    MOCK_METHOD(void, CheckCounterInfo, (D3D11_COUNTER_INFO*), (override));
    MOCK_METHOD(HRESULT,CheckCounter,
        (const D3D11_COUNTER_DESC*,
            D3D11_COUNTER_TYPE*,
            UINT*,
            LPSTR,
            UINT*,
            LPSTR,
            UINT*,
            LPSTR,
            UINT*));
    MOCK_METHOD(HRESULT,CheckFeatureSupport,
        (D3D11_FEATURE, void*, UINT), (override));
    MOCK_METHOD(HRESULT,GetPrivateData, (const GUID&, UINT*, void*), (override));
    MOCK_METHOD(HRESULT,SetPrivateData, (const GUID&, UINT, const void*), (override));
    MOCK_METHOD(HRESULT,SetPrivateDataInterface,
        (const GUID&, const IUnknown*), (override));
    MOCK_METHOD(D3D_FEATURE_LEVEL, GetFeatureLevel, (), (override));
    MOCK_METHOD(UINT, GetCreationFlags, (), (override));
    MOCK_METHOD(HRESULT,GetDeviceRemovedReason, ());
    MOCK_METHOD(void, GetImmediateContext, (ID3D11DeviceContext**), (override));
    MOCK_METHOD(HRESULT,SetExceptionMode, (UINT));
    MOCK_METHOD(UINT,GetExceptionMode, (), (override));
};*/

/*class MockID3D11DeviceContext : public ID3D11DeviceContext {
public:
    // IUnknown methods
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject) {
        return E_NOINTERFACE;
    }
    STDMETHOD_(ULONG, AddRef)() {
        return 1;
    }
    STDMETHOD_(ULONG, Release)() {
        return 1;
    }

    // Add mock methods for the ID3D11DeviceContext  methods you want to mock
};*/


class MockIPixelShader : public IPixelShader {
public:
    MOCK_METHOD(bool, Initialize, (Microsoft::WRL::ComPtr<ID3D11Device>, HWND), (override));
    MOCK_METHOD(void, Shutdown, (), (override));
    MOCK_METHOD(bool, Render, (Microsoft::WRL::ComPtr<ID3D11DeviceContext>, int vertexCount, const Light&), (override));
    MOCK_METHOD(Microsoft::WRL::ComPtr<ID3D11PixelShader>, GetPixelShader, (), (override));
};

class MockIVertexShader : public IVertexShader {
public:
    MOCK_METHOD(bool, Initialize, (Microsoft::WRL::ComPtr<ID3D11Device>, HWND), (override));
    MOCK_METHOD(void, Shutdown, (), (override));
    MOCK_METHOD(bool, Render, (Microsoft::WRL::ComPtr<ID3D11DeviceContext>, DirectX::XMMATRIX worldmat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat, DirectX::XMFLOAT4 color), (override));
    MOCK_METHOD(Microsoft::WRL::ComPtr<ID3D11VertexShader>, GetVertexShader, (), (override));
    MOCK_METHOD(Microsoft::WRL::ComPtr<ID3D11InputLayout>, GetInputLayout, (), (override));
};

#endif