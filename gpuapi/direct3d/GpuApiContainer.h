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
#include "../../../EngineDependencies/directx/d3d12.h"
#include "../../../EngineDependencies/directx/d3dx12.h"
#include <windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <wrl.h>

struct GpuApiContainer {
    uint32 frames_in_flight;
    uint32 framebuffer_idx;

    Microsoft::WRL::ComPtr<ID3D12Device> device;
    Microsoft::WRL::ComPtr<IDXGISwapChain4> swapchain;

    Microsoft::WRL::ComPtr<ID3D12CommandQueue> command_queue;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtv_heap;
    uint32 rtv_info_size;
    // @todo should be dynamic size based on frames_in_flight, no?
    Microsoft::WRL::ComPtr<ID3D12Resource> render_targets[2];
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> command_allocator;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> command_list;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipeline_state;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> root_signature;
    Microsoft::WRL::ComPtr<ID3D12Fence> fence;
    UINT64 fence_value = 0;
    HANDLE fence_event;

    // ????
    CD3DX12_VIEWPORT m_viewport;
    CD3DX12_RECT m_scissorRect;

    // @todo This definately doesn't belong here
    Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
};

#endif