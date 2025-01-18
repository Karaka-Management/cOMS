/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_GPUAPI_VULKAN_UTILS_H
#define TOS_GPUAPI_VULKAN_UTILS_H

#if _WIN32
    #ifndef VK_USE_PLATFORM_WIN32_KHR
        #define VK_USE_PLATFORM_WIN32_KHR 1
    #endif

    #include <vulkan/vulkan_win32.h>
    #include "../../platform/win32/Window.h"
#elif __linux__
    #include <vulkan/vulkan_xlib.h>
#endif

#include <vulkan/vulkan.h>
#include <vector>
#include "../../stdlib/Types.h"
#include "../../utils/StringUtils.h"
#include "../../utils/TestUtils.h"
#include "../../log/Log.h"
#include "../../memory/RingMemory.h"
#include "ShaderUtils.h"

PACKED_STRUCT;
// WARNING: indices values start at one (are offset by +1) because 0 means no value in our implementation
// The reason for the packing is that sometimes we want to use it as an array
// I am only packing it on the off chance there is some funky behaviour.
struct VulkanQueueFamilyIndices {
    uint32 graphics_family;
    uint32 present_family;
};
UNPACKED_STRUCT;

struct VulkanSwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;

    uint32 format_size;
    VkSurfaceFormatKHR* formats;

    uint32 present_mode_size;
    VkPresentModeKHR* present_modes;
};

inline
void change_viewport(Window* w, int32 offset_x = 0, int32 offset_y = 0)
{
    // @todo implement
}

int32 vulkan_check_validation_layer_support(const char** validation_layers, uint32 validation_layer_count, RingMemory* ring) {
    uint32 layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, NULL);

    VkLayerProperties* available_layers = (VkLayerProperties *) ring_get_memory(ring, layer_count * sizeof(VkLayerProperties));
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers);

    for (int32 i = 0; i < validation_layer_count; ++i) {
        bool layerFound = false;

        for (uint32 j = 0; j < layer_count; ++j) {
            if (str_compare(validation_layers[i], available_layers[j].layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return -(i + 1);
        }
    }

    return 0;
}

int32 vulkan_check_extension_support(const char** extensions, uint32 extension_count, RingMemory* ring) {
    uint32 ext_count;
    vkEnumerateInstanceExtensionProperties(NULL, &ext_count, NULL);

    VkExtensionProperties* available_extensions = (VkExtensionProperties *) ring_get_memory(ring, ext_count * sizeof(VkExtensionProperties));
    vkEnumerateInstanceExtensionProperties(NULL, &ext_count, available_extensions);

    for (int32 i = 0; i < extension_count; ++i) {
        bool layerFound = false;

        for (uint32 j = 0; j < ext_count; ++j) {
            if (str_compare(extensions[i], available_extensions[j].extensionName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return -(i + 1);
        }
    }

    return 0;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL vulkan_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT,
    const VkDebugUtilsMessengerCallbackDataEXT* debug_callback_data, void*
) {
    if ((severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        || (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    ) {
        LOG(true, debug_callback_data->pMessage);
    }

    return VK_FALSE;
}

void vulkan_populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT* create_info)
{
    create_info->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    create_info->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    create_info->pfnUserCallback = vulkan_debug_callback;
}

VkResult vulkan_debug_utils_messenger_create(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* create_info,
    const VkAllocationCallbacks* allocator, VkDebugUtilsMessengerEXT* debug_messenger
) {
    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    if (!func) {
        ASSERT_SIMPLE(func);
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    return func(instance, create_info, allocator, debug_messenger);
}

void vulkan_debug_messenger_setup(VkInstance instance, VkDebugUtilsMessengerEXT* debug_messenger)
{
    VkDebugUtilsMessengerCreateInfoEXT create_info = {};
    vulkan_populate_debug_messenger_create_info(&create_info);

    if (vulkan_debug_utils_messenger_create(instance, &create_info, NULL, debug_messenger) != VK_SUCCESS) {
        ASSERT_SIMPLE(false);
    }
}

void vulkan_instance_create(
    VkInstance* instance, const char** extensions, uint32 extension_count,
    const char** validation_layers = NULL, uint32 validation_layer_count = 0,
    RingMemory* ring = NULL
) {
    int32 err;
    if (validation_layer_count
        && (err = vulkan_check_validation_layer_support(validation_layers, validation_layer_count, ring))
    ) {
        LOG_FORMAT(true, "Vulkan validation_layer missing: %d", {{LOG_DATA_CHAR_STR, (void *) validation_layers[-err - 1]}});
        ASSERT_SIMPLE(false);
    }

    if (extension_count
        && (err = vulkan_check_extension_support(extensions, extension_count, ring))
    ) {
        LOG_FORMAT(true, "Vulkan extension missing: %d", {{LOG_DATA_CHAR_STR, (void *) extensions[-err - 1]}});
        ASSERT_SIMPLE(false);
    }

    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "TalesOfSouls";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;
    //create_info.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    create_info.enabledExtensionCount = extension_count;
    create_info.ppEnabledExtensionNames = extensions;

    VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {};
    if (validation_layer_count) {
        create_info.enabledLayerCount = validation_layer_count;
        create_info.ppEnabledLayerNames = validation_layers;

        vulkan_populate_debug_messenger_create_info(&debug_create_info);
        create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debug_create_info;
    }

    VkResult result;
    if ((result = vkCreateInstance(&create_info, NULL, instance)) != VK_SUCCESS) {
        LOG_FORMAT(true, "Vulkan vkCreateInstance: %d", LOG_DATA_INT32, (int32 *) &result);
        ASSERT_SIMPLE(false);
    }
}

void vulkan_surface_create(VkInstance instance, VkSurfaceKHR* surface, Window* window)
{
    #if _WIN32
        VkWin32SurfaceCreateInfoKHR surface_create_info = {};
        surface_create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        surface_create_info.hwnd = window->hwnd;
        surface_create_info.hinstance = window->hInstance;

        VkResult result;
        if ((result = vkCreateWin32SurfaceKHR(instance, &surface_create_info, NULL, surface)) != VK_SUCCESS) {
            LOG_FORMAT(true, "Vulkan vkCreateWin32SurfaceKHR: %d", LOG_DATA_INT32, (int32 *) &result);
            return;
        }
    #elif __linux__
    #endif
}

bool vulkan_device_supports_extensions(VkPhysicalDevice device, const char** device_extensions, uint32 device_extension_count, RingMemory* ring) {
    uint32 extension_count;
    vkEnumerateDeviceExtensionProperties(device, NULL, &extension_count, NULL);

    VkExtensionProperties* available_extensions = (VkExtensionProperties *) ring_get_memory(ring, extension_count * sizeof(VkExtensionProperties));
    vkEnumerateDeviceExtensionProperties(device, NULL, &extension_count, available_extensions);

    for (int32 i = 0; i < device_extension_count; ++i) {
        bool found = false;
        for (int32 j = 0; j < extension_count; ++j) {
            if (str_compare(device_extensions[i], available_extensions[j].extensionName) == 0) {
                found = true;
                break;
            }
        }

        if (!found) {
            return false;
        }
    }

    return true;
}

// @todo Allow to fill array
void vulkan_available_layers(RingMemory* ring) {
    uint32 layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, NULL);

    VkLayerProperties* available_layers = (VkLayerProperties *) ring_get_memory(ring, layer_count * sizeof(VkLayerProperties));
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers);
}

// @todo Allow to fill array
void vulkan_available_extensions(RingMemory* ring) {
    uint32 extension_count;
    vkEnumerateInstanceExtensionProperties(NULL, &extension_count, NULL);

    VkExtensionProperties* available_extensions = (VkExtensionProperties *) ring_get_memory(ring, extension_count * sizeof(VkExtensionProperties));
    vkEnumerateInstanceExtensionProperties(NULL, &extension_count, available_extensions);
}

VulkanQueueFamilyIndices vulkan_find_queue_families(VkPhysicalDevice physical_device, VkSurfaceKHR surface, RingMemory* ring)
{
    VulkanQueueFamilyIndices indices = {};
    uint32 queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, NULL);

    VkQueueFamilyProperties* queue_families = (VkQueueFamilyProperties *) ring_get_memory(ring, (queue_family_count + 1) * sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families);

    for (int32 i = 0; i < queue_family_count; ++i) {
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphics_family = i + 1;
        }

        VkBool32 present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &present_support);

        if (present_support) {
            indices.present_family = i + 1;
        }

        if (indices.graphics_family && indices.present_family) {
            break;
        }
    }

    return indices;
}

// WARNING: Since we use a RingMemory for the data, we need to copy it if we want it to be persistent
VulkanSwapChainSupportDetails vulkan_query_swap_chain_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface, RingMemory* ring)
{
    VulkanSwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &details.capabilities);

    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &details.format_size, NULL);

    if (details.format_size) {
        details.formats = (VkSurfaceFormatKHR *) ring_get_memory(ring, (details.format_size + 1) * sizeof(VkSurfaceFormatKHR));
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &details.format_size, details.formats);
    }

    vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &details.present_mode_size, NULL);

    if (details.present_mode_size) {
        details.present_modes = (VkPresentModeKHR *) ring_get_memory(ring, (details.present_mode_size + 1) * sizeof(VkPresentModeKHR));
        vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &details.present_mode_size, details.present_modes);
    }

    return details;
}

bool vulkan_is_device_suitable(VkPhysicalDevice physical_device, VkSurfaceKHR surface, const char** device_extensions, uint32 device_extension_count, RingMemory* ring)
{
    VulkanQueueFamilyIndices indices = vulkan_find_queue_families(physical_device, surface, ring);
    bool extensions_supported = vulkan_device_supports_extensions(physical_device, device_extensions, device_extension_count, ring);

    bool swap_chain_adequate = false;
    if (extensions_supported) {
        VulkanSwapChainSupportDetails swap_chain_support = vulkan_query_swap_chain_support(physical_device, surface, ring);
        swap_chain_adequate = swap_chain_support.format_size && swap_chain_support.present_modes;
    }

    return indices.graphics_family && indices.present_family
        && extensions_supported && swap_chain_adequate;
}

// @todo Do we want to implement something similar in opengl that does something vaguely different despite not really necessary? (see wglGetGPUIDs, wglCreateAssociatedContextAMD)
void gpuapi_pick_physical_device(
    VkInstance instance, VkSurfaceKHR surface, VkPhysicalDevice* physical_device,
    const char** device_extensions, uint32 device_extension_count, RingMemory* ring
) {
    uint32 device_count;
    vkEnumeratePhysicalDevices(instance, &device_count, NULL);

    VkPhysicalDevice* devices = (VkPhysicalDevice *) ring_get_memory(ring, device_count * sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(instance, &device_count, devices);

    for (int32 i = 0; i < device_count; ++i) {
        if (vulkan_is_device_suitable(devices[i], surface, device_extensions, device_extension_count, ring)) {
            // @question Do we really have to do a memcpy or could we just assign? Isn't VkPhysicalDevice just a pointer internally?
            memcpy(physical_device, &devices[i], sizeof(VkPhysicalDevice));
            return;
        }
    }

    LOG(true, "Vulkan failed to find physical device");
    ASSERT_SIMPLE(false);
}

void gpuapi_create_logical_device(
    VkInstance instance, VkSurfaceKHR surface, VkDevice* device, VkPhysicalDevice physical_device,
    VkQueue* graphics_queue, VkQueue* present_queue,
    const char** device_extensions, uint32 device_extension_count,
    const char** validation_layers, uint32 validation_layer_count, RingMemory* ring
) {
    VulkanQueueFamilyIndices indices = vulkan_find_queue_families(physical_device, surface, ring);

    VkDeviceQueueCreateInfo* queue_create_infos = (VkDeviceQueueCreateInfo *) ring_get_memory(ring, 2 * sizeof(VkDeviceQueueCreateInfo), 4, true);

    f32 queue_priority = 1.0f;
    uint32 queue_create_info_count = 1;
    queue_create_infos[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_infos[0].queueFamilyIndex = indices.graphics_family;
    queue_create_infos[0].queueCount = 1;
    queue_create_infos[0].pQueuePriorities = &queue_priority;

    if (indices.present_family != indices.graphics_family) {
        queue_create_infos[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_infos[1].queueFamilyIndex = indices.present_family;
        queue_create_infos[1].queueCount = 1;
        queue_create_infos[1].pQueuePriorities = &queue_priority;
        ++queue_create_info_count;
    }

    VkPhysicalDeviceFeatures device_features = {};
    VkDeviceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.queueCreateInfoCount = queue_create_info_count;
    create_info.pQueueCreateInfos = queue_create_infos;
    create_info.pEnabledFeatures = &device_features;
    create_info.enabledExtensionCount = device_extension_count;
    create_info.ppEnabledExtensionNames = device_extensions;

    if (validation_layers) {
        create_info.enabledLayerCount = validation_layer_count;
        create_info.ppEnabledLayerNames = validation_layers;
    } else {
        create_info.enabledLayerCount = 0;
    }

    VkResult result;
    if ((result = vkCreateDevice(physical_device, &create_info, NULL, device)) != VK_SUCCESS) {
        LOG_FORMAT(true, "Vulkan vkCreateDevice: %d", LOG_DATA_INT32, (int32 *) &result);
        ASSERT_SIMPLE(false);
    }

    vkGetDeviceQueue(*device, indices.graphics_family, 0, graphics_queue);
    vkGetDeviceQueue(*device, indices.present_family, 0, present_queue);
}

// WARNING: swapchain_images needs to already have reserved enough memory
// @todo How can we ensure swapchain_images has enough but not too much space?
// @question Do we need to handle old swapchains?
void vulkan_swap_chain_create(
    VkDevice device, VkPhysicalDevice physical_device, VkSurfaceKHR surface,
    VkSwapchainKHR* swapchain, VkFormat* swapchain_image_format, VkExtent2D* swapchain_extent,
    Window* window, RingMemory* ring
) {
    VulkanSwapChainSupportDetails swap_chain_support = vulkan_query_swap_chain_support(physical_device, surface, ring);

    VkSurfaceFormatKHR* surface_format = &swap_chain_support.formats[0];
    for (int32 i = 0; i < swap_chain_support.format_size; ++i) {
        if (swap_chain_support.formats[i].format == VK_FORMAT_B8G8R8A8_SRGB
            && swap_chain_support.formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
        ) {
            surface_format = &swap_chain_support.formats[i];
            break;
        }
    }

    // @todo switch from VK_PRESENT_MODE_MAILBOX_KHR to VK_PRESENT_MODE_FIFO_KHR
    VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
    for (int32 i = 0; i < swap_chain_support.present_mode_size; ++i) {
        if (swap_chain_support.present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            present_mode = swap_chain_support.present_modes[i];
            break;
        }
    }

    if (swap_chain_support.capabilities.currentExtent.width != UINT32_MAX) {
        *swapchain_extent = swap_chain_support.capabilities.currentExtent;
    } else {
        swapchain_extent->width = OMS_CLAMP(
            window->width,
            swap_chain_support.capabilities.maxImageExtent.width,
            swap_chain_support.capabilities.minImageExtent.width
        );

        swapchain_extent->height = OMS_CLAMP(
            window->height,
            swap_chain_support.capabilities.maxImageExtent.height,
            swap_chain_support.capabilities.minImageExtent.height
        );
    }

    uint32 image_count = swap_chain_support.capabilities.minImageCount + 1;
    if (swap_chain_support.capabilities.maxImageCount > 0
        && image_count > swap_chain_support.capabilities.maxImageCount
    ) {
        image_count = swap_chain_support.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = surface;
    create_info.minImageCount = image_count;
    create_info.imageFormat = surface_format->format;
    create_info.imageColorSpace = surface_format->colorSpace;
    create_info.imageExtent = *swapchain_extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    VulkanQueueFamilyIndices indices = vulkan_find_queue_families(physical_device, surface, ring);

    if (indices.graphics_family != indices.present_family) {
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = (uint32 *) &indices;
    } else {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    create_info.preTransform = swap_chain_support.capabilities.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = present_mode;
    create_info.clipped = VK_TRUE;
    create_info.oldSwapchain = VK_NULL_HANDLE;

    VkResult result;
    if ((result = vkCreateSwapchainKHR(device, &create_info, NULL, swapchain)) != VK_SUCCESS) {
        LOG_FORMAT(true, "Vulkan vkCreateSwapchainKHR: %d", LOG_DATA_INT32, (int32 *) &result);
        ASSERT_SIMPLE(false);
    }

    memcpy(swapchain_image_format, &surface_format->format, sizeof(VkFormat));
}

void vulkan_swap_chain_images_create(
    VkDevice device, VkSwapchainKHR swapchain,
    VkImage** swapchain_images, uint32* swapchain_image_count,
    BufferMemory* buf
) {
    vkGetSwapchainImagesKHR(device, swapchain, swapchain_image_count, NULL);
    // @question Do we really want to allocate in here or should that be done by the caller
    *swapchain_images = (VkImage *) buffer_get_memory(buf, *swapchain_image_count * sizeof(VkImage), 4);

    vkGetSwapchainImagesKHR(device, swapchain, swapchain_image_count, *swapchain_images);
}

void vulkan_image_views_create(
    VkDevice device, VkImageView* swapchain_image_views,
    VkImage* swapchain_images, uint32 swapchain_image_count, VkFormat swapchain_image_format
) {
    VkResult result;
    for (size_t i = 0; i < swapchain_image_count; ++i) {
        VkImageViewCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        create_info.image = swapchain_images[i];
        create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        create_info.format = swapchain_image_format;
        create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        create_info.subresourceRange.baseMipLevel = 0;
        create_info.subresourceRange.levelCount = 1;
        create_info.subresourceRange.baseArrayLayer = 0;
        create_info.subresourceRange.layerCount = 1;

        if ((result = vkCreateImageView(device, &create_info, NULL, &swapchain_image_views[i])) != VK_SUCCESS) {
            LOG_FORMAT(true, "Vulkan vkCreateImageView: %d", LOG_DATA_INT32, (int32 *) &result);
            ASSERT_SIMPLE(false);
        }
    }
}

void create_render_pass(
    VkDevice device, VkRenderPass* render_pass, VkFormat swapchain_image_format
) {
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = swapchain_image_format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    VkResult result;
    if ((result = vkCreateRenderPass(device, &renderPassInfo, NULL, render_pass)) != VK_SUCCESS) {
        LOG_FORMAT(true, "Vulkan vkCreateRenderPass: %d", LOG_DATA_INT32, (int32 *) &result);
        ASSERT_SIMPLE(false);
    }
}

// @todo This is very similar to program_make in opengl. Consider to rename opengl
void vulkan_pipeline_create(
    VkDevice device,
    VkShaderModule vertex_shader,
    VkShaderModule fragment_shader,
    VkShaderModule geometry_shader,
    VkPipeline* pipeline,
    VkPipelineLayout* pipeline_layout,
    VkRenderPass render_pass,
    const char* source,
    int32 source_size
)
{
    uint32 stage_count = 0;
    VkPipelineShaderStageCreateInfo shaderStages[3];

    VkPipelineShaderStageCreateInfo vs_stage_create = {};
    vs_stage_create.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vs_stage_create.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vs_stage_create.module = vertex_shader;
    vs_stage_create.pName = "main";
    ++stage_count;

    VkPipelineShaderStageCreateInfo fs_stage_create = {};
    fs_stage_create.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fs_stage_create.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fs_stage_create.module = fragment_shader;
    fs_stage_create.pName = "main";
    ++stage_count;

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    VkDynamicState dynamicStates[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = ARRAY_COUNT(dynamicStates);
    dynamicState.pDynamicStates = dynamicStates;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pushConstantRangeCount = 0;

    VkResult result;
    if ((result = vkCreatePipelineLayout(device, &pipelineLayoutInfo, NULL, pipeline_layout)) != VK_SUCCESS) {
        LOG_FORMAT(true, "Vulkan vkCreatePipelineLayout: %d", LOG_DATA_INT32, (int32 *) &result);
        ASSERT_SIMPLE(false);
    }

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = stage_count;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = *pipeline_layout;
    pipelineInfo.renderPass = render_pass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if ((result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, pipeline)) != VK_SUCCESS) {
        LOG_FORMAT(true, "Vulkan vkCreateGraphicsPipelines: %d", LOG_DATA_INT32, (int32 *) &result);
        ASSERT_SIMPLE(false);
    }

    vkDestroyShaderModule(device, vertex_shader, NULL);
    vkDestroyShaderModule(device, fragment_shader, NULL);
}

// @todo consider to rename to same name as opengl
// WARNING: swapchain_framebuffers needs to be initialized
void vulkan_framebuffer_create(
    VkDevice device, VkFramebuffer* swapchain_framebuffers,
    VkImageView* swapchain_image_views, uint32 swapchain_image_count, VkExtent2D swapchain_extent,
    VkRenderPass render_pass
) {
    VkResult result;
    for (uint32 i = 0; i < swapchain_image_count; i++) {
        VkImageView attachments[] = {
            swapchain_image_views[i]
        };

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = render_pass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapchain_extent.width;
        framebufferInfo.height = swapchain_extent.height;
        framebufferInfo.layers = 1;

        if ((result = vkCreateFramebuffer(device, &framebufferInfo, NULL, &swapchain_framebuffers[i])) != VK_SUCCESS) {
            LOG_FORMAT(true, "Vulkan vkCreateFramebuffer: %d", LOG_DATA_INT32, (int32 *) &result);
            ASSERT_SIMPLE(false);
        }
    }
}

void vulkan_command_pool_create(
    VkDevice device, VkCommandPool* command_pool,
    VkPhysicalDevice physical_device, VkSurfaceKHR surface, RingMemory* ring
) {
    VulkanQueueFamilyIndices queue_family_indices = vulkan_find_queue_families(physical_device, surface, ring);

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queue_family_indices.graphics_family;

    VkResult result;
    if ((result = vkCreateCommandPool(device, &poolInfo, NULL, command_pool)) != VK_SUCCESS) {
        LOG_FORMAT(true, "Vulkan vkCreateCommandPool: %d", LOG_DATA_INT32, (int32 *) &result);
        ASSERT_SIMPLE(false);
    }
}

void vulkan_command_buffer_create(VkDevice device, VkCommandBuffer* command_buffer, VkCommandPool command_pool)
{
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = command_pool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    VkResult result;
    if ((result = vkAllocateCommandBuffers(device, &allocInfo, command_buffer)) != VK_SUCCESS) {
        LOG_FORMAT(true, "Vulkan vkAllocateCommandBuffers: %d", LOG_DATA_INT32, (int32 *) &result);
        ASSERT_SIMPLE(false);
    }
}

void vulkan_sync_objects_create(VkDevice device, VkSemaphore* image_available_semaphore, VkSemaphore* render_finished_semaphore, VkFence* in_flight_fence)
{
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VkResult result;
    if ((result = vkCreateSemaphore(device, &semaphoreInfo, NULL, image_available_semaphore)) != VK_SUCCESS
        || (result = vkCreateSemaphore(device, &semaphoreInfo, NULL, render_finished_semaphore)) != VK_SUCCESS
        || (result = vkCreateFence(device, &fenceInfo, NULL, in_flight_fence)) != VK_SUCCESS
    ) {
        LOG_FORMAT(true, "Vulkan vulkan_sync_objects_create: %d", LOG_DATA_INT32, (int32 *) &result);

        ASSERT_SIMPLE(false);
    }
}
/*
void vulkan_command_buffer_record(
     VkCommandBuffer command_buffer
) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(command_buffer, &beginInfo) != VK_SUCCESS) {
        ASSERT_SIMPLE(false);
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChainExtent;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swapChainExtent.width);
    viewport.height = static_cast<float>(swapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    vkCmdDraw(commandBuffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        ASSERT_SIMPLE(false);
    }
}
*/
#endif