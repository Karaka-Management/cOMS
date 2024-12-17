/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_GPUAPI_DIRECTX_GPU_API_CONTAINER
#define TOS_GPUAPI_DIRECTX_GPU_API_CONTAINER

#include <windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <wrl.h>

struct GpuApiContainer {
    Microsoft::WRL::ComPtr<ID3D12Device> device;
    Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain;

    Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap;
    Microsoft::WRL::ComPtr<ID3D12Resource> renderTargets[2];
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
    Microsoft::WRL::ComPtr<ID3D12Fence> fence;
    UINT64 fenceValue = 0;
};

#endif