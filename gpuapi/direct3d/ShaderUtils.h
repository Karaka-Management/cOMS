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

#include <d3d12.h>
#include <d3dcompiler.h>

#include "../../stdlib/Types.h"
#include "../../memory/RingMemory.h"
#include "../../log/Log.h"

D3D12_SHADER_BYTECODE shader_make(ID3D12Device* device, const char* source, int32 source_size, RingMemory* ring)
{
    // Create the shader object (bytecode)
    D3D12_SHADER_BYTECODE shaderBytecodeDesc = {};
    shaderBytecodeDesc.pShaderBytecode = source;
    shaderBytecodeDesc.BytecodeLength = source_size;

    return shaderBytecodeDesc;
}

ID3D12PipelineState* program_make(
    ID3D12Device* device,
    D3D12_SHADER_BYTECODE vertex_shader,
    D3D12_SHADER_BYTECODE fragment_shader,
    D3D12_SHADER_BYTECODE geometry_shader
) {
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.VS = vertex_shader;
    psoDesc.PS = fragment_shader;
    psoDesc.GS = geometry_shader;

    ID3D12PipelineState* pipelineState = NULL;
    HRESULT hr = device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState));

    if (FAILED(hr)) {
        LOG("Failed to create program", true, true);
        return NULL;
    }

    return pipelineState;
}

inline
void pipeline_use(ID3D12GraphicsCommandList* command_list, ID3D12PipelineState* pipelineState)
{
    command_list->SetPipelineState(pipelineState);
}

#endif