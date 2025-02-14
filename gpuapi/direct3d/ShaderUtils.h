/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_GPUAPI_DIRECTX_SHADER_UTILS_H
#define TOS_GPUAPI_DIRECTX_SHADER_UTILS_H

#include <windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include "../../../EngineDependencies/directx/d3d12.h"
#include "../../../EngineDependencies/directx/d3dx12.h"

#include "../../stdlib/Types.h"
#include "../../memory/RingMemory.h"
#include "../../log/Log.h"
#include "../ShaderType.h"

#pragma comment(lib, "d3dcompiler.lib")

const char* shader_type_index(ShaderType type)
{
    switch (type) {
        case SHADER_TYPE_VERTEX:
            return "vs_5_0";
        case SHADER_TYPE_FRAGMENT:
            return "ps_5_0";
        default:
            UNREACHABLE();
    }
}

Microsoft::WRL::ComPtr<ID3DBlob> shader_make(const char* type, const char* source, int32 source_size)
{
    #if DEBUG || INTERNAL
        uint32 compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    #else
        uint32 compileFlags = 0;
    #endif

    Microsoft::WRL::ComPtr<ID3DBlob> blob;
    Microsoft::WRL::ComPtr<ID3DBlob> errMsgs;
    if (FAILED(D3DCompile2(source, source_size, NULL, NULL, NULL, "main", type, compileFlags, 0, 0, NULL, 0, blob.GetAddressOf(), errMsgs.GetAddressOf()))) {
        LOG(true, "DirectX12 D3DCompile2");
        ASSERT_SIMPLE(false);
    }

    return blob;
}

ID3D12PipelineState* program_make(
    ID3D12Device* device,
    Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipeline_state,
    ID3D12RootSignature* root_signature,
    ID3DBlob* vertex_shader,
    ID3DBlob* fragment_shader,
    ID3DBlob*
) {
    // @todo We need to find a way to do this somewhere else:
    D3D12_INPUT_ELEMENT_DESC input_element_info[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeline_state_info = {};
    pipeline_state_info.InputLayout = { input_element_info, _countof(input_element_info) };
    pipeline_state_info.pRootSignature = root_signature;
    pipeline_state_info.VS = CD3DX12_SHADER_BYTECODE(vertex_shader);
    pipeline_state_info.PS = CD3DX12_SHADER_BYTECODE(fragment_shader);
    pipeline_state_info.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    pipeline_state_info.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    pipeline_state_info.DepthStencilState.DepthEnable = FALSE;
    pipeline_state_info.DepthStencilState.StencilEnable = FALSE;
    pipeline_state_info.SampleMask = UINT_MAX;
    pipeline_state_info.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    pipeline_state_info.NumRenderTargets = 1;
    pipeline_state_info.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    pipeline_state_info.SampleDesc.Count = 1;

    if (FAILED(device->CreateGraphicsPipelineState(&pipeline_state_info, IID_PPV_ARGS(&pipeline_state)))) {
        LOG(true, "DirectX12 CreateGraphicsPipelineState");
        ASSERT_SIMPLE(false);
    }

    return pipeline_state.Get();
}

inline
void pipeline_use(ID3D12GraphicsCommandList* command_list, ID3D12PipelineState* pipelineState)
{
    command_list->SetPipelineState(pipelineState);
}

#endif