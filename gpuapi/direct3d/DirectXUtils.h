/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_GPUAPI_DIRECTX_UTILS_H
#define TOS_GPUAPI_DIRECTX_UTILS_H

#include <windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcommon.h>
#include "../../../GameEngine/log/Log.h"
// #include "../../../EngineDependencies/directx/d3d12.h"
// #include "../../../EngineDependencies/directx/d3dx12.h"

#include "../../stdlib/Types.h"

// A more (compile-time) efficient version of the windows macro IID_PPV_ARGS
#define IID_PPVOID(pointer) __uuidof(**(pointer)), (void **) (pointer)

bool is_directx_supported(D3D_FEATURE_LEVEL version)
{
    IDXGIFactory6* factory = NULL;
    if (FAILED(CreateDXGIFactory1(IID_PPVOID(&factory)))) {
        return false;
    }

    bool is_dx12_supported = false;

    IDXGIAdapter1* adapter = NULL;
    for (uint32 i = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(i, &adapter); ++i) {
        DXGI_ADAPTER_DESC1 desc;
        if (FAILED(adapter->GetDesc1(&desc))) {
            adapter->Release();
            continue;
        }

        // Skip software adapters
        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            adapter->Release();
            continue;
        }

        // Check for DirectX 12 support
        if (SUCCEEDED(D3D12CreateDevice(adapter, version, _uuidof(ID3D12Device), NULL))) {
            is_dx12_supported = true;
            adapter->Release();
            break;
        }

        adapter->Release();
    }

    factory->Release();

    return is_dx12_supported;
}

int32 max_directx_version()
{
    if (is_directx_supported(D3D_FEATURE_LEVEL_12_2)) {
        return 122;
    } else if (is_directx_supported(D3D_FEATURE_LEVEL_12_1)) {
        return 121;
    } else if (is_directx_supported(D3D_FEATURE_LEVEL_12_0)) {
        return 120;
    } else if (is_directx_supported(D3D_FEATURE_LEVEL_11_1)) {
        return 111;
    } else if (is_directx_supported(D3D_FEATURE_LEVEL_11_0)) {
        return 110;
    } else if (is_directx_supported(D3D_FEATURE_LEVEL_10_1)) {
        return 101;
    } else if (is_directx_supported(D3D_FEATURE_LEVEL_10_0)) {
        return 100;
    } else if (is_directx_supported(D3D_FEATURE_LEVEL_9_3)) {
        return 93;
    } else if (is_directx_supported(D3D_FEATURE_LEVEL_9_2)) {
        return 92;
    } else if (is_directx_supported(D3D_FEATURE_LEVEL_9_1)) {
        return 91;
    } else if (is_directx_supported(D3D_FEATURE_LEVEL_1_0_CORE)) {
        return 90;
    }

    return 0;
}

// Returns frame index
int32 wait_for_previous_frame(
    ID3D12Fence* fence, HANDLE fence_event, UINT64* fence_value,
    ID3D12CommandQueue* graphics_queue, IDXGISwapChain3* swapchain
)
{
    // WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
    // This is code implemented as such for simplicity. The D3D12HelloFrameBuffering
    // sample illustrates how to use fences for efficient resource usage and to
    // maximize GPU utilization.

    UINT64 fence_value_temp = *fence_value;

    // Signal and increment the fence value.
    if(FAILED(graphics_queue->Signal(fence, fence_value_temp))) {
        LOG(true, "DirectX12 Signal");
        ASSERT_SIMPLE(false);
    }

    ++(*fence_value);

    // Wait until the previous frame is finished.
    if (fence->GetCompletedValue() < fence_value_temp) {
        if (FAILED(fence->SetEventOnCompletion(fence_value_temp, fence_event))) {
            LOG(true, "DirectX12 SetEventOnCompletion");
            ASSERT_SIMPLE(false);
        }

        WaitForSingleObject(fence_event, INFINITE);
    }

    return swapchain->GetCurrentBackBufferIndex();
}

static
void directx_debug_callback(
    D3D12_MESSAGE_CATEGORY category,
    D3D12_MESSAGE_SEVERITY severity,
    D3D12_MESSAGE_ID id,
    LPCSTR description,
    void* context
) {
    // @todo handle severity
    (void) category;
    (void) severity;
    (void) id;
    (void*) context;
    /*
    if ((severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        || (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    ) {

    }
    */

    LOG(true, description);
    ASSERT_SIMPLE(false);
}

void gpuapi_debug_messenger_setup(ID3D12Device* device)
{
    ID3D12InfoQueue1* info_queue;
    if (FAILED(device->QueryInterface(IID_PPVOID(&info_queue)))) {
        return;
    }

    // Register the custom debug callback
    info_queue->RegisterMessageCallback(
        directx_debug_callback,
        D3D12_MESSAGE_CALLBACK_FLAG_NONE,
        NULL, // Context (can be used to pass additional data)
        NULL // Callback cookie (unused)
    );

    // Set the message count limit to unlimited
    info_queue->SetMessageCountLimit(0);

    info_queue->Release();
}

inline
void gpuapi_create_logical_device(ID3D12Device** device) {
    if (FAILED(D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPVOID(device)))) {
        LOG(true, "DirectX12 D3D12CreateDevice");
        ASSERT_SIMPLE(false);
    }
}

#endif