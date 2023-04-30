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
    MockID3D11DeviceContext() = default;
    ~MockID3D11DeviceContext() =default ;

    // IUnknown methods
    STDMETHOD_(ULONG, AddRef)() {
        return 1;
    }
    STDMETHOD_(ULONG, Release)() {
        return 1;
    }

    MOCK_METHOD(HRESULT, QueryInterface, (REFIID riid, void** ppv), (override));
    // ID3D11DevieChild methods.
    MOCK_METHOD(void, GetDevice, (ID3D11Device** ppDevice), (override));
    MOCK_METHOD(HRESULT, GetPrivateData,
        (REFGUID guid, UINT* pDataSize, void* pData), (override));
    MOCK_METHOD(HRESULT, SetPrivateData,
        (REFGUID guid, UINT DataSize, const void* pData), (override));
    MOCK_METHOD(HRESULT,SetPrivateDataInterface,
        (REFGUID guid, const IUnknown* pData), (override));
    // ID3D11DeviceContext methods.
    MOCK_METHOD(void, VSSetConstantBuffers,
        (UINT StartSlot,
            UINT NumBuffers,
            ID3D11Buffer* const* ppConstantBuffers), (override));
    MOCK_METHOD(void,
        PSSetShaderResources,
        (UINT StartSlot,
            UINT NumViews,
            ID3D11ShaderResourceView* const* ppShaderResourceViews), (override));
    MOCK_METHOD(void, PSSetShader,
        (ID3D11PixelShader* pPixelShader,
            ID3D11ClassInstance* const* ppClassInstances,
            UINT NumClassInstances), (override));
    MOCK_METHOD(void, PSSetSamplers,
        (UINT StartSlot,
            UINT NumSamplers,
            ID3D11SamplerState* const* ppSamplers), (override));
    MOCK_METHOD(void, VSSetShader,
        (ID3D11VertexShader* pVertexShader,
            ID3D11ClassInstance* const* ppClassInstances,
            UINT NumClassInstances), (override));
    MOCK_METHOD(void, DrawIndexed,
        (UINT IndexCount,
            UINT StartIndexLocation,
            INT BaseVertexLocation), (override));
    MOCK_METHOD(void, Draw, (UINT VertexCount, UINT StartVertexLocation), (override));
    MOCK_METHOD(HRESULT, Map,
        (ID3D11Resource* pResource,
            UINT Subresource,
            D3D11_MAP MapType,
            UINT MapFlags,
            D3D11_MAPPED_SUBRESOURCE* pMappedResource), (override));
    MOCK_METHOD(void, Unmap,
        (ID3D11Resource* pResource, UINT Subresource), (override));
    MOCK_METHOD(void, PSSetConstantBuffers,
        (UINT StartSlot,
            UINT NumBuffers,
            ID3D11Buffer* const* ppConstantBuffers), (override));
    MOCK_METHOD(void, IASetInputLayout, (ID3D11InputLayout* pInputLayout), (override));
    MOCK_METHOD(void, IASetVertexBuffers,
        (UINT StartSlot,
            UINT NumBuffers,
            ID3D11Buffer* const* ppVertexBuffers,
            const UINT* pStrides,
            const UINT* pOffsets), (override));
    MOCK_METHOD(void, IASetIndexBuffer,
        (ID3D11Buffer* pIndexBuffer,
            DXGI_FORMAT Format,
            UINT Offset), (override));
    MOCK_METHOD(void, DrawIndexedInstanced,
        (UINT IndexCountPerInstance,
            UINT InstanceCount,
            UINT StartIndexLocation,
            INT BaseVertexLocation,
            UINT StartInstanceLocation), (override));
    MOCK_METHOD(void, DrawInstanced,
        (UINT VertexCountPerInstance,
            UINT InstanceCount,
            UINT StartVertexLocation,
            UINT StartInstanceLocation), (override));
    MOCK_METHOD(void, GSSetConstantBuffers,
        (UINT StartSlot,
            UINT NumBuffers,
            ID3D11Buffer* const* ppConstantBuffers), (override));
    MOCK_METHOD(void, GSSetShader,
        (ID3D11GeometryShader* pShader,
            ID3D11ClassInstance* const* ppClassInstances,
            UINT NumClassInstances), (override));
    MOCK_METHOD(void, IASetPrimitiveTopology,
        (D3D11_PRIMITIVE_TOPOLOGY Topology), (override));
    MOCK_METHOD(void, 
        VSSetShaderResources,
        (UINT StartSlot,
            UINT NumViews,
            ID3D11ShaderResourceView* const* ppShaderResourceViews), (override));
    MOCK_METHOD(void, VSSetSamplers,
        (UINT StartSlot,
            UINT NumSamplers,
            ID3D11SamplerState* const* ppSamplers), (override));
    MOCK_METHOD(void, Begin, (ID3D11Asynchronous* pAsync), (override));
    MOCK_METHOD(void, End, (ID3D11Asynchronous* pAsync), (override));
    MOCK_METHOD(HRESULT, GetData,
        (ID3D11Asynchronous* pAsync,
            void* pData,
            UINT DataSize,
            UINT GetDataFlags), (override));
    MOCK_METHOD(void, SetPredication,
        (ID3D11Predicate* pPredicate, BOOL PredicateValue), (override));
    MOCK_METHOD(void, 
        GSSetShaderResources,
        (UINT StartSlot,
            UINT NumViews,
            ID3D11ShaderResourceView* const* ppShaderResourceViews), (override));
    MOCK_METHOD(void, GSSetSamplers,
        (UINT StartSlot,
            UINT NumSamplers,
            ID3D11SamplerState* const* ppSamplers), (override));
    MOCK_METHOD(void, OMSetRenderTargets,
        (UINT NumViews,
            ID3D11RenderTargetView* const* ppRenderTargetViews,
            ID3D11DepthStencilView* pDepthStencilView), (override));
    MOCK_METHOD(void,
        OMSetRenderTargetsAndUnorderedAccessViews,
        (UINT NumRTVs,
            ID3D11RenderTargetView* const* ppRenderTargetViews,
            ID3D11DepthStencilView* pDepthStencilView,
            UINT UAVStartSlot,
            UINT NumUAVs,
            ID3D11UnorderedAccessView* const* ppUnorderedAccessViews,
            const UINT* pUAVInitialCounts), (override));
    MOCK_METHOD(void, OMSetBlendState,
        (ID3D11BlendState* pBlendState,
            const FLOAT BlendFactor[4],
            UINT SampleMask), (override));
    MOCK_METHOD(void, OMSetDepthStencilState,
        (ID3D11DepthStencilState* pDepthStencilState,
            UINT StencilRef), (override));
    MOCK_METHOD(void, SOSetTargets,
        (UINT NumBuffers,
            ID3D11Buffer* const* ppSOTargets,
            const UINT* pOffsets), (override));
    MOCK_METHOD(void, DrawAuto, (), (override));
    MOCK_METHOD(void, DrawIndexedInstancedIndirect,
        (ID3D11Buffer* pBufferForArgs,
            UINT AlignedByteOffsetForArgs), (override));
    MOCK_METHOD(void, DrawInstancedIndirect,
        (ID3D11Buffer* pBufferForArgs,
            UINT AlignedByteOffsetForArgs), (override));
    MOCK_METHOD(void, Dispatch,
        (UINT ThreadGroupCountX,
            UINT ThreadGroupCountY,
            UINT ThreadGroupCountZ), (override));
    MOCK_METHOD(void, DispatchIndirect,
        (ID3D11Buffer* pBufferForArgs,
            UINT AlignedByteOffsetForArgs), (override));
    MOCK_METHOD(void, RSSetState,
        (ID3D11RasterizerState* pRasterizerState), (override));
    MOCK_METHOD(void, RSSetViewports,
        (UINT NumViewports,
            const D3D11_VIEWPORT* pViewports), (override));
    MOCK_METHOD(void, RSSetScissorRects,
        (UINT NumRects, const D3D11_RECT* pRects), (override));
    MOCK_METHOD(void, CopySubresourceRegion,
        (ID3D11Resource* pDstResource,
            UINT DstSubresource,
            UINT DstX,
            UINT DstY,
            UINT DstZ,
            ID3D11Resource* pSrcResource,
            UINT SrcSubresource,
            const D3D11_BOX* pSrcBox), (override));
    MOCK_METHOD(void, CopyResource,
        (ID3D11Resource* pDstResource,
            ID3D11Resource* pSrcResource), (override));
    MOCK_METHOD(void, UpdateSubresource,
        (ID3D11Resource* pDstResource,
            UINT DstSubresource,
            const D3D11_BOX* pDstBox,
            const void* pSrcData,
            UINT SrcRowPitch,
            UINT SrcDepthPitch), (override));
    MOCK_METHOD(void, CopyStructureCount,
        (ID3D11Buffer* pDstBuffer,
            UINT DstAlignedByteOffset,
            ID3D11UnorderedAccessView* pSrcView), (override));
    MOCK_METHOD(void, ClearRenderTargetView,
        (ID3D11RenderTargetView* pRenderTargetView,
            const FLOAT ColorRGBA[4]), (override));
    MOCK_METHOD(void, ClearUnorderedAccessViewUint,
        (ID3D11UnorderedAccessView* pUnorderedAccessView,
            const UINT Values[4]), (override));
    MOCK_METHOD(void, ClearUnorderedAccessViewFloat,
        (ID3D11UnorderedAccessView* pUnorderedAccessView,
            const FLOAT Values[4]), (override));
    MOCK_METHOD(void, ClearDepthStencilView,
        (ID3D11DepthStencilView* pDepthStencilView,
            UINT ClearFlags,
            FLOAT Depth,
            UINT8 Stencil), (override));
    MOCK_METHOD(void, GenerateMips,
        (ID3D11ShaderResourceView* pShaderResourceView), (override));
    MOCK_METHOD(void, SetResourceMinLOD,
        (ID3D11Resource* pResource, FLOAT MinLOD), (override));
    MOCK_METHOD(FLOAT, GetResourceMinLOD, (ID3D11Resource* pResource), (override));
    MOCK_METHOD(void, ResolveSubresource,
        (ID3D11Resource* pDstResource,
            UINT DstSubresource,
            ID3D11Resource* pSrcResource,
            UINT SrcSubresource,
            DXGI_FORMAT Format), (override));
    MOCK_METHOD(void, ExecuteCommandList,
        (ID3D11CommandList* pCommandList,
            BOOL RestoreContextState), (override));
    MOCK_METHOD(void,
        HSSetShaderResources,
        (UINT StartSlot,
            UINT NumViews,
            ID3D11ShaderResourceView* const* ppShaderResourceViews), (override));
    MOCK_METHOD(void, HSSetShader,
        (ID3D11HullShader* pHullShader,
            ID3D11ClassInstance* const* ppClassInstances,
            UINT NumClassInstances), (override));
    MOCK_METHOD(void, HSSetSamplers,
        (UINT StartSlot,
            UINT NumSamplers,
            ID3D11SamplerState* const* ppSamplers), (override));
    MOCK_METHOD(void, HSSetConstantBuffers,
        (UINT StartSlot,
            UINT NumBuffers,
            ID3D11Buffer* const* ppConstantBuffers), (override));
    MOCK_METHOD( void, 
        DSSetShaderResources,
        (UINT StartSlot,
            UINT NumViews,
            ID3D11ShaderResourceView* const* ppShaderResourceViews), (override));
    MOCK_METHOD( void, DSSetShader,
        (ID3D11DomainShader* pDomainShader,
            ID3D11ClassInstance* const* ppClassInstances,
            UINT NumClassInstances), (override));
    MOCK_METHOD( void, DSSetSamplers,
        (UINT StartSlot,
            UINT NumSamplers,
            ID3D11SamplerState* const* ppSamplers), (override));
    MOCK_METHOD( void, DSSetConstantBuffers,
        (UINT StartSlot,
            UINT NumBuffers,
            ID3D11Buffer* const* ppConstantBuffers), (override));
    MOCK_METHOD( void, 
        CSSetShaderResources,
        (UINT StartSlot,
            UINT NumViews,
            ID3D11ShaderResourceView* const* ppShaderResourceViews), (override));
    MOCK_METHOD( void,
        CSSetUnorderedAccessViews,
        (UINT StartSlot,
            UINT NumUAVs,
            ID3D11UnorderedAccessView* const* ppUnorderedAccessViews,
            const UINT* pUAVInitialCounts), (override));
    MOCK_METHOD( void, CSSetShader,
        (ID3D11ComputeShader* pComputeShader,
            ID3D11ClassInstance* const* ppClassInstances,
            UINT NumClassInstances), (override));
    MOCK_METHOD( void, CSSetSamplers,
        (UINT StartSlot,
            UINT NumSamplers,
            ID3D11SamplerState* const* ppSamplers), (override));
    MOCK_METHOD( void, CSSetConstantBuffers,
        (UINT StartSlot,
            UINT NumBuffers,
            ID3D11Buffer* const* ppConstantBuffers), (override));
    MOCK_METHOD( void, VSGetConstantBuffers,
        (UINT StartSlot,
            UINT NumBuffers,
            ID3D11Buffer** ppConstantBuffers), (override));
    MOCK_METHOD( void, PSGetShaderResources,
        (UINT StartSlot,
            UINT NumViews,
            ID3D11ShaderResourceView** ppShaderResourceViews), (override));
    MOCK_METHOD( void, PSGetShader,
        (ID3D11PixelShader** ppPixelShader,
            ID3D11ClassInstance** ppClassInstances,
            UINT* pNumClassInstances), (override));
    MOCK_METHOD( void, PSGetSamplers,
        (UINT StartSlot,
            UINT NumSamplers,
            ID3D11SamplerState** ppSamplers), (override));
    MOCK_METHOD( void, VSGetShader,
        (ID3D11VertexShader** ppVertexShader,
            ID3D11ClassInstance** ppClassInstances,
            UINT* pNumClassInstances), (override));
    MOCK_METHOD( void, PSGetConstantBuffers,
        (UINT StartSlot,
            UINT NumBuffers,
            ID3D11Buffer** ppConstantBuffers), (override));
    MOCK_METHOD( void, IAGetInputLayout,
        (ID3D11InputLayout** ppInputLayout), (override));
    MOCK_METHOD( void, IAGetVertexBuffers,
        (UINT StartSlot,
            UINT NumBuffers,
            ID3D11Buffer** ppVertexBuffers,
            UINT* pStrides,
            UINT* pOffsets), (override));
    MOCK_METHOD( void, IAGetIndexBuffer,
        (ID3D11Buffer** pIndexBuffer,
            DXGI_FORMAT* Format,
            UINT* Offset), (override));
    MOCK_METHOD( void, GSGetConstantBuffers,
        (UINT StartSlot,
            UINT NumBuffers,
            ID3D11Buffer** ppConstantBuffers), (override));
    MOCK_METHOD( void, GSGetShader,
        (ID3D11GeometryShader** ppGeometryShader,
            ID3D11ClassInstance** ppClassInstances,
            UINT* pNumClassInstances), (override));
    MOCK_METHOD( void, IAGetPrimitiveTopology,
        (D3D11_PRIMITIVE_TOPOLOGY* pTopology), (override));
    MOCK_METHOD( void, VSGetShaderResources,
        (UINT StartSlot,
            UINT NumViews,
            ID3D11ShaderResourceView** ppShaderResourceViews), (override));
    MOCK_METHOD( void, VSGetSamplers,
        (UINT StartSlot,
            UINT NumSamplers,
            ID3D11SamplerState** ppSamplers), (override));
    MOCK_METHOD( void, GetPredication,
        (ID3D11Predicate** ppPredicate,
            BOOL* pPredicateValue), (override));
    MOCK_METHOD( void, GSGetShaderResources,
        (UINT StartSlot,
            UINT NumViews,
            ID3D11ShaderResourceView** ppShaderResourceViews), (override));
    MOCK_METHOD( void, GSGetSamplers,
        (UINT StartSlot,
            UINT NumSamplers,
            ID3D11SamplerState** ppSamplers), (override));
    MOCK_METHOD( void, OMGetRenderTargets,
        (UINT NumViews,
            ID3D11RenderTargetView** ppRenderTargetViews,
            ID3D11DepthStencilView** ppDepthStencilView), (override));
    MOCK_METHOD( void,
        OMGetRenderTargetsAndUnorderedAccessViews,
        (UINT NumRTVs,
            ID3D11RenderTargetView** ppRenderTargetViews,
            ID3D11DepthStencilView** ppDepthStencilView,
            UINT UAVStartSlot,
            UINT NumUAVs,
            ID3D11UnorderedAccessView** ppUnorderedAccessViews), (override));
    MOCK_METHOD( void, OMGetBlendState,
        (ID3D11BlendState** ppBlendState,
            FLOAT BlendFactor[4],
            UINT* pSampleMask), (override));
    MOCK_METHOD( void, OMGetDepthStencilState,
        (ID3D11DepthStencilState** ppDepthStencilState,
            UINT* pStencilRef), (override));
    MOCK_METHOD( void, SOGetTargets,
        (UINT NumBuffers, ID3D11Buffer** ppSOTargets), (override));
    MOCK_METHOD( void, RSGetState,
        (ID3D11RasterizerState** ppRasterizerState), (override));
    MOCK_METHOD( void, RSGetViewports,
        (UINT* pNumViewports, D3D11_VIEWPORT* pViewports), (override));
    MOCK_METHOD( void, RSGetScissorRects,
        (UINT* pNumRects, D3D11_RECT* pRects), (override));
    MOCK_METHOD( void, HSGetShaderResources,
        (UINT StartSlot,
            UINT NumViews,
            ID3D11ShaderResourceView** ppShaderResourceViews), (override));
    MOCK_METHOD( void, HSGetShader,
        (ID3D11HullShader** ppHullShader,
            ID3D11ClassInstance** ppClassInstances,
            UINT* pNumClassInstances), (override));
    MOCK_METHOD( void, HSGetSamplers,
        (UINT StartSlot,
            UINT NumSamplers,
            ID3D11SamplerState** ppSamplers), (override));
    MOCK_METHOD( void, HSGetConstantBuffers,
        (UINT StartSlot,
            UINT NumBuffers,
            ID3D11Buffer** ppConstantBuffers), (override));
    MOCK_METHOD( void, DSGetShaderResources,
        (UINT StartSlot,
            UINT NumViews,
            ID3D11ShaderResourceView** ppShaderResourceViews), (override));
    MOCK_METHOD( void, DSGetShader,
        (ID3D11DomainShader** ppDomainShader,
            ID3D11ClassInstance** ppClassInstances,
            UINT* pNumClassInstances), (override));
    MOCK_METHOD( void, DSGetSamplers,
        (UINT StartSlot,
            UINT NumSamplers,
            ID3D11SamplerState** ppSamplers), (override));
    MOCK_METHOD( void, DSGetConstantBuffers,
        (UINT StartSlot,
            UINT NumBuffers,
            ID3D11Buffer** ppConstantBuffers), (override));
    MOCK_METHOD( void, CSGetShaderResources,
        (UINT StartSlot,
            UINT NumViews,
            ID3D11ShaderResourceView** ppShaderResourceViews), (override));
    MOCK_METHOD( void, 
        CSGetUnorderedAccessViews,
        (UINT StartSlot,
            UINT NumUAVs,
            ID3D11UnorderedAccessView** ppUnorderedAccessViews), (override));
    MOCK_METHOD( void, CSGetShader,
        (ID3D11ComputeShader** ppComputeShader,
            ID3D11ClassInstance** ppClassInstances,
            UINT* pNumClassInstances), (override));
    MOCK_METHOD( void, CSGetSamplers,
        (UINT StartSlot,
            UINT NumSamplers,
            ID3D11SamplerState** ppSamplers), (override));
    MOCK_METHOD( void, CSGetConstantBuffers,
        (UINT StartSlot,
            UINT NumBuffers,
            ID3D11Buffer** ppConstantBuffers), (override));
    MOCK_METHOD( void, ClearState, (), (override));
    MOCK_METHOD( void, Flush, (), (override));
    MOCK_METHOD(D3D11_DEVICE_CONTEXT_TYPE, GetType, (), (override));
    MOCK_METHOD(UINT, GetContextFlags, (), (override));
    MOCK_METHOD(HRESULT, FinishCommandList,
        (BOOL RestoreDeferredContextState,
            ID3D11CommandList** ppCommandList), (override));
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

class MockIModelSubscriber : public IModelSubscriber
{
public:
    MOCK_METHOD(void, HandleIModelState, (const MeshGroupState&), (override));
    MOCK_METHOD(void, HandleIModelState, (const FlythroughState&), (override));
    MOCK_METHOD(void, HandleIModelState, (const Explore3DState&), (override));
    MOCK_METHOD(void, HandleIModelState, (const GeneralModelState&), (override));
};

class MockIPixelShader : public IPixelShader {
public:
    MOCK_METHOD(bool, Initialize, (Microsoft::WRL::ComPtr<ID3D11Device>), (override));
    MOCK_METHOD(void, Shutdown, (), (override));
    MOCK_METHOD(bool, Render, (Microsoft::WRL::ComPtr<ID3D11DeviceContext>, int vertexCount, const Light&), (override));
    MOCK_METHOD(Microsoft::WRL::ComPtr<ID3D11PixelShader>, GetPixelShader, (), (override));
};

class MockPixelShaderMesh : public PixelShaderMesh {
public:
    MOCK_METHOD(bool, Initialize, (Microsoft::WRL::ComPtr<ID3D11Device>), (override));
    MOCK_METHOD(void, Shutdown, (), (override));
    MOCK_METHOD(bool, Render, (Microsoft::WRL::ComPtr<ID3D11DeviceContext>, int vertexCount, const Light&), (override));
    MOCK_METHOD(Microsoft::WRL::ComPtr<ID3D11PixelShader>, GetPixelShader, (), (override));
    MOCK_METHOD(bool, GetIsShadingOn, (), (const));
    MOCK_METHOD(void, SetIsShadingOn, (bool), ());
};

class MockIVertexShader : public IVertexShader {
public:
    MOCK_METHOD(bool, Initialize, (Microsoft::WRL::ComPtr<ID3D11Device>), (override));
    MOCK_METHOD(void, Shutdown, (), (override));
    MOCK_METHOD(bool, Render, (Microsoft::WRL::ComPtr<ID3D11DeviceContext>, DirectX::XMMATRIX worldmat, DirectX::XMMATRIX viewMat, DirectX::XMMATRIX projectionMat, DirectX::XMFLOAT4 color), (override));
    MOCK_METHOD(Microsoft::WRL::ComPtr<ID3D11VertexShader>, GetVertexShader, (), (override));
    MOCK_METHOD(Microsoft::WRL::ComPtr<ID3D11InputLayout>, GetInputLayout, (), (override));
};

template <class V>
class MockIRenderable : public IRenderable<V> {
public:
    MOCK_METHOD(bool, Initialize, (Microsoft::WRL::ComPtr<ID3D11Device>, IVertexShaderPtr, IPixelShaderPtr, V*, unsigned long*, UINT, UINT), (override));
    MOCK_METHOD(void, Shutdown, (), (override));
    MOCK_METHOD(void, Render, (Microsoft::WRL::ComPtr<ID3D11DeviceContext>, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX, const Light&), (override));
    MOCK_METHOD(void, SetName, (const std::wstring&), (override));
    MOCK_METHOD(void, Rotate, (float, float, float), (override));
    MOCK_METHOD(void, Translate, (float, float, float), (override));
    MOCK_METHOD(void, Scale, (float, float, float), (override));
    MOCK_METHOD(void, ResetTransformation, (), (override));
    MOCK_METHOD(void, SetColor, (float, float, float, float), (override));
    MOCK_METHOD(void, SetIsSeen, (bool), (override));
    MOCK_METHOD(bool, IsSeen, (), (const override));
    MOCK_METHOD(IRenderableState, GetState, (), (const override));
    MOCK_METHOD(std::wstring, GetName, (), (override));
    MOCK_METHOD(DirectX::XMMATRIX, GetWorldMatrix, (), (override));
};

class MockCameraPositioner : public CameraPositioner {
public:
    MOCK_METHOD(void, MoveForward, (float dt), ());
    MOCK_METHOD(void, MoveBack, (float dt), ());
    MOCK_METHOD(void, MoveLeft, (float dt), ());
    MOCK_METHOD(void, MoveRight, (float dt), ());
    MOCK_METHOD(void, MoveUp, (float dt), ());
    MOCK_METHOD(void, MoveDown, (float dt), ());
    MOCK_METHOD(void, RotatePitchYaw, (float x, float y), ());
    MOCK_METHOD(void, Initialize, (CameraPtr camera), ());
    MOCK_METHOD(void, SetSpeed, (float speed), ());
    MOCK_METHOD(void, SetRotationSpeed, (float speed), ());
    MOCK_METHOD(float, GetSpeed, (), (const ));
    MOCK_METHOD(float, GetRotationSpeed, (), (const ));
    MOCK_METHOD(EpochTime, GetCurrentEpochTime, (), (const ));
    MOCK_METHOD(void, SetCurrentEpochTime, (EpochTime), ());
};

class MockLight : public Light {
public:
    MOCK_METHOD(void, UpdateSunPosition, (std::time_t currentEpochTime, double lat, double longitude), ());
    MOCK_METHOD(void, SetDiffuseColor, (const DirectX::XMFLOAT4& diffuseColor), ());
    MOCK_METHOD(void, SetAmbientColor, (const DirectX::XMFLOAT4& ambientColor), ());
    MOCK_METHOD(DirectX::XMFLOAT4, GetDiffuseColor, (), (const ));
    MOCK_METHOD(DirectX::XMFLOAT4, GetAmbientColor, (), (const ));
    MOCK_METHOD(DirectX::XMFLOAT4, GetInverseDirection, (), (const ));
    MOCK_METHOD(double, GetAzimuth, (), (const ));
    MOCK_METHOD(double, GetElevation, (), (const ));
};

class MockCameraTrajectory : public CameraTrajectory{
public:
    MOCK_METHOD(bool, Initialize, (const std::vector<CameraPose>&cameraPoses, IRendarablePtr<VertexPolyLine>  renderable, CameraPtr camera), ());
    MOCK_METHOD(bool, IsInitialized, (), (const ));
    MOCK_METHOD(bool, UpdateCamera, (double elapsedmsecs), ());
    MOCK_METHOD(void, ResetStartPosition, (), ());
    MOCK_METHOD(void, Clear, (), ());
    MOCK_METHOD(void, Shutdown, (), ());
    MOCK_METHOD(void, SetStartEpochTime,(EpochTime), ());
    MOCK_METHOD(void, SetCurrentFrame,(unsigned frameNum), ());
    MOCK_METHOD(void, SetSpeed,(float speed), ());
    MOCK_METHOD(float, GetSpeed, (), (const ));
    MOCK_METHOD(EpochTime, GetCurrentEpochTime, (), (const ));
    MOCK_METHOD(EpochTime, GetStartEpochTime, (), (const ));
    MOCK_METHOD(unsigned, GetCurrentFrameNum, (), (const ));
    MOCK_METHOD(unsigned, GetNumberOfFrame, (), (const ));
    MOCK_METHOD(IRenderableState , GetTrajectoryPolyLineState ,(),(const ));
    MOCK_METHOD(IRendarablePtr<VertexPolyLine>, GetPolyLine ,(),(const ));
};
class MockIDataAccess : public IDataAccess {
public:
    MOCK_METHOD(void, LoadTerrain_withSharpEdges, (const wchar_t*), (override));
    MOCK_METHOD(const std::vector<stlFacet>&, GetFacets, (), (override));
    MOCK_METHOD(void, LoadCameraTrajectory, (const wchar_t*, std::vector<CameraPose>&), (override));
    MOCK_METHOD(void, LoadConfigurationFile, (const wchar_t*, ParameterFile& params), (override));
    MOCK_METHOD(void, LoadTerrain_withSoftEdges, (const wchar_t*), (override));
    MOCK_METHOD(const std::vector<StlVertex>&, GetVertices_Soft, (), (override));
    MOCK_METHOD(const std::vector<CornerIndices>&, GetIndices_Soft, (), (override));
};

class MockModelMessageSystem : public ModelMessageSystem {
public:
    MOCK_METHOD(void, PublishModelState, (const MeshGroupState&), (const ));
    MOCK_METHOD(void, PublishModelState, (const FlythroughState&), (const ));
    MOCK_METHOD(void, PublishModelState, (const Explore3DState&), (const ));
    MOCK_METHOD(void, PublishModelState, (const GeneralModelState&), (const ));
    MOCK_METHOD(bool, Subscribe, (IModelSubscriberPtr view), ());
    MOCK_METHOD(bool, Unsubscribe, (IModelSubscriberPtr view), ());
};

class MockCamera : public Camera {
public:
    MOCK_METHOD(void, Initialize, (int screenWidth, int screenHeight, float screenNear, float screenDepth, float fieldOfView), ());
    MOCK_METHOD(void, Resize, (int screenWidth, int screenHeight), ());
    MOCK_METHOD(void, Reset, (), ());
    MOCK_METHOD(void, SetFieldOfView, (float fovRad), ());
    MOCK_METHOD(void, SetNearScreen, (float nearZ), ());
    MOCK_METHOD(void, SetFarScreen, (float farZ), ());
    MOCK_METHOD(void, SetPosition, (float x, float y, float z), ());
    MOCK_METHOD(void, SetRotationRad, (float x, float y, float z), ());
    MOCK_METHOD(void, AdjustPosition, (float deltaX, float deltaY, float deltaZ), ());
    MOCK_METHOD(void, AdjustRotationRad, (float deltaX, float deltaY, float deltaZ), ());
    MOCK_METHOD(Vector3D, GetPositionVec, (), (const ));
    MOCK_METHOD(Vector3D, GetRotationVec, (), (const ));
    MOCK_METHOD(DirectX::XMFLOAT3, GetPositionF3, (), (const ));
    MOCK_METHOD(DirectX::XMFLOAT3, GetRotationF3, (), (const ));
    MOCK_METHOD(void, Render, (), ());
    MOCK_METHOD(DirectX::XMMATRIX, GetViewMatrix, (), (const ));
    MOCK_METHOD(DirectX::XMMATRIX, GetProjectionMatrix, (), (const ));
    MOCK_METHOD(DirectX::XMMATRIX, GetRotationMatrix, (), (const ));
    MOCK_METHOD(float, GetFOVrad, (), (const ));
    MOCK_METHOD(float, GetNearScreen, (), (const ));
    MOCK_METHOD(float, GetFarScreen, (), (const ));

};



#endif