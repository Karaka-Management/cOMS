/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_GPUAPI_DIRECT3D_UTILS
#define TOS_GPUAPI_DIRECT3D_UTILS

#include <windows.h>
#include <wrl.h>

#include <dxgi1_6.h>
#include "../../../EngineDependencies/directx/d3d12.h"
#include "../../../EngineDependencies/directx/d3dx12.h"

#include "../../stdlib/Types.h"

#define FRAME_COUNT 2

struct Window {
    bool is_fullscreen;
    int32 width;
    int32 height;
    char name[32];

    int32 x;
    int32 y;

    HWND hwnd;

    // @todo move this out of here to a separate gpuapi struct (same with opengl)
    Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain;

    Microsoft::WRL::ComPtr<ID3D12Device> device;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargets[FRAME_COUNT];
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;
    Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;

    UINT m_rtvDescriptorSize;

    UINT m_frameIndex;
    HANDLE m_fenceEvent;
    UINT64 m_fenceValue;
};

void window_create(Window* window, void* proc)
{
    WNDPROC wndproc = (WNDPROC) proc;
    WNDCLASSEX wc = {};
    HINSTANCE hinstance = GetModuleHandle(0);

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = wndproc;
    wc.hInstance = hinstance;
    wc.lpszClassName = (LPCWSTR) window->name;

    RegisterClassEx(&wc);

    if (window->is_fullscreen) {
        window->width  = GetSystemMetrics(SM_CXSCREEN);
	    window->height = GetSystemMetrics(SM_CYSCREEN);

        DEVMODE screen_settings;

        memset(&screen_settings, 0, sizeof(screen_settings));
		screen_settings.dmSize       = sizeof(screen_settings);
		screen_settings.dmPelsWidth  = (unsigned long) window->width;
		screen_settings.dmPelsHeight = (unsigned long) window->height;
		screen_settings.dmBitsPerPel = 32;
		screen_settings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&screen_settings, CDS_FULLSCREEN);

        window->x = 0;
        window->y = 0;
    }

    window->hwnd = CreateWindowEx((DWORD) NULL,
        wc.lpszClassName, NULL,
        WS_OVERLAPPEDWINDOW,
        window->x, window->y,
        window->width,
        window->height,
        NULL, NULL, hinstance, window
    );

    //SetWindowLongA(window->hwnd, GWL_STYLE, 0);
}

void window_open(const Window* window)
{
    ShowWindow(window->hwnd, SW_SHOW);
    SetForegroundWindow(window->hwnd);
	SetFocus(window->hwnd);
    ShowCursor(false);
    UpdateWindow(window->hwnd);
}

void window_close(Window* window)
{
    CloseWindow(window->hwnd);
}

bool is_directx_12_supported()
{
    Microsoft::WRL::ComPtr<IDXGIFactory6> factory;
    HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
    if (FAILED(hr)) {
        return false;
    }

    Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
    for (UINT adapterIndex = 0;
        DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(adapterIndex, &adapter);
        ++adapterIndex)
    {
        DXGI_ADAPTER_DESC1 desc;
        adapter->GetDesc1(&desc);

        // Skip software adapters
        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            continue;
        }

        try {
            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))) {
                return true;
            }
        } catch (...) {
            return false;
        }
    }

    return false;
}

void find_hardware_adapter(
    IDXGIFactory1* factory,
    IDXGIAdapter1** adapter1,
    bool use_high_performance_adapter = true
) {
    *adapter1 = nullptr;

    Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
    Microsoft::WRL::ComPtr<IDXGIFactory6> factory6;

    if (SUCCEEDED(factory->QueryInterface(IID_PPV_ARGS(&factory6)))) {
        for (
            UINT adapterIndex = 0;
            SUCCEEDED(factory6->EnumAdapterByGpuPreference(
                adapterIndex,
                use_high_performance_adapter == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
                IID_PPV_ARGS(&adapter)));
            ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
                // Don't select the Basic Render Driver adapter.
                continue;
            }

            // Check to see whether the adapter supports Direct3D 12, but don't create the
            // actual device yet.
            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))) {
                break;
            }
        }
    }

    if(adapter.Get() == nullptr) {
        for (UINT adapterIndex = 0; SUCCEEDED(factory->EnumAdapters1(adapterIndex, &adapter)); ++adapterIndex) {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
                // Don't select the Basic Render Driver adapter.
                continue;
            }

            // Check to see whether the adapter supports Direct3D 12, but don't create the
            // actual device yet.
            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))) {
                break;
            }
        }
    }

    *adapter1 = adapter.Detach();
}

void load_pipeline(Window* window)
{
    uint32 factory_flags = 0;

    Microsoft::WRL::ComPtr<IDXGIFactory4> factory;
    CreateDXGIFactory2(factory_flags, IID_PPV_ARGS(&factory));

    Microsoft::WRL::ComPtr<IDXGIAdapter1> hardware_adapter;
    find_hardware_adapter(factory.Get(), &hardware_adapter);

    D3D12CreateDevice(
        hardware_adapter.Get(),
        D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS(&window->device)
    );

    // Describe and create the command queue.
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    window->device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&window->m_commandQueue));

    // Describe and create the swap chain.
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = FRAME_COUNT;
    swapChainDesc.Width = window->width;
    swapChainDesc.Height = window->height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;
    factory->CreateSwapChainForHwnd(
        window->m_commandQueue.Get(),        // Swap chain needs the queue so that it can force a flush on it.
        window->hwnd,
        &swapChainDesc,
        nullptr,
        nullptr,
        &swapChain
    );

    // This sample does not support fullscreen transitions.
    factory->MakeWindowAssociation(window->hwnd, DXGI_MWA_NO_ALT_ENTER);

    swapChain.As(&window->m_swapChain);
    window->m_frameIndex = window->m_swapChain->GetCurrentBackBufferIndex();

    // Create descriptor heaps.
    {
        // Describe and create a render target view (RTV) descriptor heap.
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
        rtvHeapDesc.NumDescriptors = FRAME_COUNT;
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        window->device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&window->m_rtvHeap));

        window->m_rtvDescriptorSize = window->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }

    // Create frame resources.
    {
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(window->m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

        // Create a RTV for each frame.
        for (UINT n = 0; n < FRAME_COUNT; n++)
        {
            window->m_swapChain->GetBuffer(n, IID_PPV_ARGS(&window->m_renderTargets[n]));
            window->device->CreateRenderTargetView(window->m_renderTargets[n].Get(), nullptr, rtvHandle);
            rtvHandle.Offset(1, window->m_rtvDescriptorSize);
        }
    }

    window->device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&window->m_commandAllocator));
}

void load_assets(Window* window)
{
    // Create the command list.
    window->device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, window->m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&window->m_commandList));

    // Command lists are created in the recording state, but there is nothing
    // to record yet. The main loop expects it to be closed, so close it now.
    window->m_commandList->Close();

    // Create synchronization objects.
    {
        window->device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&window->m_fence));
        window->m_fenceValue = 1;

        // Create an event handle to use for frame synchronization.
        window->m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (window->m_fenceEvent == nullptr) {
            HRESULT_FROM_WIN32(GetLastError());
        }
    }
}

#endif