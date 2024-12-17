/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_GPUAPI_VULKAN_GPU_API_CONTAINER
#define TOS_GPUAPI_VULKAN_GPU_API_CONTAINER

#include <vulkan/vulkan.h>

struct GpuApiContainer {
    VkInstance instance;
    VkSurfaceKHR surface;
    VkDevice device;
    VkSwapchainKHR swapchain;
    VkPipelineLayout pipelineLayout;
    VkFramebuffer framebuffer;
    VkQueue graphicsQueue;
    VkRenderPass renderPass;
    VkPipeline pipeline;
    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;
};

#endif