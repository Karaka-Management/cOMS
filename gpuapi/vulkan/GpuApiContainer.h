/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_GPUAPI_VULKAN_GPU_API_CONTAINER_H
#define TOS_GPUAPI_VULKAN_GPU_API_CONTAINER_H

#include "../../stdlib/Types.h"
#include <vulkan/vulkan.h>

#define FRAME_LAG 2

struct GpuApiContainer {
    VkInstance instance;
    VkSurfaceKHR surface;
    VkDevice device;
    VkSwapchainKHR swapchain;
    uint32 swapchain_image_count;
    VkFormat swapchain_image_format;
    VkImage* swapchain_images;             // length = swapchain_image_count
    VkImageView* swapchain_image_views;    // length = swapchain_image_count
    VkFramebuffer* framebuffers; // length = swapchain_image_count
    VkExtent2D swapchain_extent;
    VkPipelineLayout pipeline_layout;
    VkQueue graphics_queue;
    VkQueue present_queue;
    VkRenderPass render_pass;
    VkPipeline pipeline;
    VkCommandPool command_pool;
    VkCommandBuffer command_buffer;
    VkSemaphore image_available_semaphore;
    VkSemaphore render_finished_semaphore;
    VkFence in_flight_fence;

    #if DEBUG || INTERNAL
        VkDebugUtilsMessengerEXT debug_messenger;
    #endif
};

#endif