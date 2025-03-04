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
#include <d3dcompiler.h>

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

ID3DBlob* shader_make(const char* type, const char* source, int32 source_size)
{
    LOG_1("Create shader");
    #if DEBUG || INTERNAL
        uint32 compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    #else
        uint32 compileFlags = 0;
    #endif

    ID3DBlob* blob;
    ID3DBlob* errMsgs;
    if (FAILED(D3DCompile2(source, source_size, NULL, NULL, NULL, "main", type, compileFlags, 0, 0, NULL, 0, &blob, &errMsgs))) {
        LOG_1("DirectX12 D3DCompile2");
        ASSERT_SIMPLE(false);
    }

    if (errMsgs) {
        errMsgs->Release();
    }

    LOG_1("Created shader");

    return blob;
}

ID3D12PipelineState* pipeline_make(
    ID3D12Device* device,
    ID3D12PipelineState** pipeline,
    ID3D12RootSignature* pipeline_layout,
    ID3DBlob* vertex_shader,
    ID3DBlob* fragment_shader,
    ID3DBlob*
) {
    PROFILE(PROFILE_PIPELINE_MAKE, NULL, false, true);
    LOG_1("Create pipeline");
    // @todo We need to find a way to do this somewhere else:
    D3D12_INPUT_ELEMENT_DESC input_element_info[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeline_state_info = {};
    pipeline_state_info.InputLayout = { input_element_info, _countof(input_element_info) };
    pipeline_state_info.pRootSignature = pipeline_layout;
    pipeline_state_info.VS = {
        .pShaderBytecode = vertex_shader->GetBufferPointer(),
        .BytecodeLength = vertex_shader->GetBufferSize()
    };
    pipeline_state_info.PS = {
        .pShaderBytecode = fragment_shader->GetBufferPointer(),
        .BytecodeLength = fragment_shader->GetBufferSize()
    };
    pipeline_state_info.RasterizerState = {
        .FillMode = D3D12_FILL_MODE_SOLID,
        .CullMode = D3D12_CULL_MODE_BACK,
        .FrontCounterClockwise = false,
        .DepthBias = D3D12_DEFAULT_DEPTH_BIAS,
        .DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP,
        .SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS,
        .DepthClipEnable = true,
        .MultisampleEnable = false,
        .AntialiasedLineEnable = false,
        .ForcedSampleCount = 0,
        .ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
    };

    const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc = {
        false,false,
        D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
        D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
        D3D12_LOGIC_OP_NOOP,
        D3D12_COLOR_WRITE_ENABLE_ALL,
    };
    pipeline_state_info.BlendState = {
        .AlphaToCoverageEnable = false,
        .IndependentBlendEnable = false,
        .RenderTarget = {
            defaultRenderTargetBlendDesc, defaultRenderTargetBlendDesc, defaultRenderTargetBlendDesc, defaultRenderTargetBlendDesc,
            defaultRenderTargetBlendDesc, defaultRenderTargetBlendDesc, defaultRenderTargetBlendDesc, defaultRenderTargetBlendDesc
        }
    };
    pipeline_state_info.DepthStencilState.DepthEnable = false;
    pipeline_state_info.DepthStencilState.StencilEnable = false;
    pipeline_state_info.SampleMask = UINT_MAX;
    pipeline_state_info.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    pipeline_state_info.NumRenderTargets = 1;
    pipeline_state_info.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    pipeline_state_info.SampleDesc.Count = 1;

    if (FAILED(device->CreateGraphicsPipelineState(&pipeline_state_info, IID_PPV_ARGS(pipeline)))) {
        LOG_1("DirectX12 CreateGraphicsPipelineState");
        ASSERT_SIMPLE(false);
    }

    // @question When do I ->Release() vertex_shader and fragment_shader?
    LOG_1("Created pipeline");

    return *pipeline;
}

inline
void pipeline_use(ID3D12GraphicsCommandList* command_buffer, ID3D12PipelineState* pipelineState)
{
    command_buffer->SetPipelineState(pipelineState);
}

#endif