/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_GPUAPI_DIRECTX_GPU_API_CONTAINER_H
#define TOS_GPUAPI_DIRECTX_GPU_API_CONTAINER_H

#include "../../stdlib/Types.h"
// #include "../../../EngineDependencies/directx/d3d12.h"
// #include "../../../EngineDependencies/directx/d3dx12.h"
#include <windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

struct GpuApiContainer {
    uint32 frames_in_flight;
    uint32 framebuffer_idx;

    ID3D12Device* device;
    IDXGISwapChain4* swapchain;

    ID3D12CommandQueue* graphics_queue;
    ID3D12DescriptorHeap* rtv_heap; // basically = swapchain_image_views
    uint32 rtv_info_size;
    // @todo should be dynamic size based on frames_in_flight, no?
    ID3D12Resource* framebuffer[2];
    ID3D12CommandAllocator* command_pool;
    ID3D12GraphicsCommandList* command_buffer;
    ID3D12PipelineState* pipeline;
    ID3D12RootSignature* pipeline_layout;
    ID3D12Fence* in_flight_fence;
    UINT64 fence_value = 0;
    HANDLE fence_event;

    // ????
    D3D12_VIEWPORT m_viewport;
    D3D12_RECT m_scissorRect;

    // @todo This definitely doesn't belong here
    ID3D12Resource* m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
};

#endif