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
    int32 graphics_family;
    int32 present_family;
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
    (void *) w;
    (void) offset_x;
    (void) offset_y;
    // @todo implement
}

int32 vulkan_check_validation_layer_support(const char** validation_layers, uint32 validation_layer_count, RingMemory* ring) {
    uint32 layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, NULL);

    VkLayerProperties* available_layers = (VkLayerProperties *) ring_get_memory(ring, layer_count * sizeof(VkLayerProperties));
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers);

    for (uint32 i = 0; i < validation_layer_count; ++i) {
        bool layerFound = false;

        for (uint32 j = 0; j < layer_count; ++j) {
            if (str_compare(validation_layers[i], available_layers[j].layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return -((int32) (i + 1));
        }
    }

    return 0;
}

int32 vulkan_check_extension_support(const char** extensions, uint32 extension_count, RingMemory* ring) {
    uint32 ext_count;
    vkEnumerateInstanceExtensionProperties(NULL, &ext_count, NULL);

    VkExtensionProperties* available_extensions = (VkExtensionProperties *) ring_get_memory(ring, ext_count * sizeof(VkExtensionProperties));
    vkEnumerateInstanceExtensionProperties(NULL, &ext_count, available_extensions);

    for (uint32 i = 0; i < extension_count; ++i) {
        bool layerFound = false;

        for (uint32 j = 0; j < ext_count; ++j) {
            if (str_compare(extensions[i], available_extensions[j].extensionName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return -((int32) (i + 1));
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
        ASSERT_SIMPLE(false);
    }

    return VK_FALSE;
}

void gpuapi_debug_messenger_setup(VkInstance instance, VkDebugUtilsMessengerEXT* debug_messenger)
{
    // @question Why do I need this twice (see other definition)
    VkDebugUtilsMessengerCreateInfoEXT create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    create_info.pfnUserCallback = vulkan_debug_callback;

    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (!func) {
        ASSERT_SIMPLE(func);
        return;
    }

    if (func(instance, &create_info, NULL, debug_messenger) != VK_SUCCESS) {
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

        return;
    }

    if (extension_count
        && (err = vulkan_check_extension_support(extensions, extension_count, ring))
    ) {
        LOG_FORMAT(true, "Vulkan extension missing: %d", {{LOG_DATA_CHAR_STR, (void *) extensions[-err - 1]}});
        ASSERT_SIMPLE(false);

        return;
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

    if (validation_layer_count) {
        create_info.enabledLayerCount = validation_layer_count;
        create_info.ppEnabledLayerNames = validation_layers;

        // @question Why do I need this twice (see other definition)
        VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {};
        debug_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debug_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debug_create_info.pfnUserCallback = vulkan_debug_callback;

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
        // @todo implement
    #endif
}

bool vulkan_device_supports_extensions(VkPhysicalDevice device, const char** device_extensions, uint32 device_extension_count, RingMemory* ring) {
    uint32 extension_count;
    vkEnumerateDeviceExtensionProperties(device, NULL, &extension_count, NULL);

    VkExtensionProperties* available_extensions = (VkExtensionProperties *) ring_get_memory(ring, extension_count * sizeof(VkExtensionProperties));
    vkEnumerateDeviceExtensionProperties(device, NULL, &extension_count, available_extensions);

    for (uint32 i = 0; i < device_extension_count; ++i) {
        bool found = false;
        for (uint32 j = 0; j < extension_count; ++j) {
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
    VulkanQueueFamilyIndices indices = { -1, -1 };
    uint32 queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, NULL);

    VkQueueFamilyProperties* queue_families = (VkQueueFamilyProperties *) ring_get_memory(ring, (queue_family_count + 1) * sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families);

    for (uint32 i = 0; i < queue_family_count; ++i) {
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphics_family = i;
        }

        VkBool32 present_support = false;

        VkResult result;
        if ((result = vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &present_support)) != VK_SUCCESS) {
            LOG_FORMAT(true, "Vulkan vkGetPhysicalDeviceSurfaceSupportKHR: %d", LOG_DATA_INT32, (int32 *) &result);
            ASSERT_SIMPLE(false);

            return indices;
        }

        if (present_support) {
            indices.present_family = i;
        }

        if (indices.graphics_family >= 0 && indices.present_family >= 0) {
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

    return indices.graphics_family >= 0 && indices.present_family >= 0
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

    for (uint32 i = 0; i < device_count; ++i) {
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
    VkSurfaceKHR surface, VkDevice* device, VkPhysicalDevice physical_device,
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

// @question Do we need to handle old swapchains?
void vulkan_swap_chain_create(
    VkDevice device, VkPhysicalDevice physical_device, VkSurfaceKHR surface,
    VkSwapchainKHR* swapchain, VkFormat* swapchain_image_format, VkExtent2D* swapchain_extent,
    Window* window, RingMemory* ring
) {
    VulkanSwapChainSupportDetails swap_chain_support = vulkan_query_swap_chain_support(physical_device, surface, ring);

    VkSurfaceFormatKHR* surface_format = &swap_chain_support.formats[0];
    for (uint32 i = 0; i < swap_chain_support.format_size; ++i) {
        if (swap_chain_support.formats[i].format == VK_FORMAT_B8G8R8A8_SRGB
            && swap_chain_support.formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
        ) {
            surface_format = &swap_chain_support.formats[i];
            break;
        }
    }

    // @todo switch from VK_PRESENT_MODE_MAILBOX_KHR to VK_PRESENT_MODE_FIFO_KHR
    VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
    for (uint32 i = 0; i < swap_chain_support.present_mode_size; ++i) {
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

        return;
    }

    memcpy(swapchain_image_format, &surface_format->format, sizeof(VkFormat));
}

// WARNING: swapchain_images needs to already have reserved enough memory
// @todo How can we ensure swapchain_images has enough but not too much space?
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

void vulkan_render_pass_create(
    VkDevice device, VkRenderPass* render_pass, VkFormat swapchain_image_format
) {
    VkAttachmentDescription color_attachment = {};
    color_attachment.format = swapchain_image_format;
    color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attachment_ref = {};
    color_attachment_ref.attachment = 0;
    color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_ref;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo render_pass_info = {};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_info.attachmentCount = 1;
    render_pass_info.pAttachments = &color_attachment;
    render_pass_info.subpassCount = 1;
    render_pass_info.pSubpasses = &subpass;
    render_pass_info.dependencyCount = 1;
    render_pass_info.pDependencies = &dependency;

    VkResult result;
    if ((result = vkCreateRenderPass(device, &render_pass_info, NULL, render_pass)) != VK_SUCCESS) {
        LOG_FORMAT(true, "Vulkan vkCreateRenderPass: %d", LOG_DATA_INT32, (int32 *) &result);
        ASSERT_SIMPLE(false);
    }
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

    VkCommandPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pool_info.queueFamilyIndex = queue_family_indices.graphics_family;

    VkResult result;
    if ((result = vkCreateCommandPool(device, &pool_info, NULL, command_pool)) != VK_SUCCESS) {
        LOG_FORMAT(true, "Vulkan vkCreateCommandPool: %d", LOG_DATA_INT32, (int32 *) &result);
        ASSERT_SIMPLE(false);
    }
}

void vulkan_command_buffer_create(VkDevice device, VkCommandBuffer* command_buffer, VkCommandPool command_pool)
{
    VkCommandBufferAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = command_pool;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = 1;

    VkResult result;
    if ((result = vkAllocateCommandBuffers(device, &alloc_info, command_buffer)) != VK_SUCCESS) {
        LOG_FORMAT(true, "Vulkan vkAllocateCommandBuffers: %d", LOG_DATA_INT32, (int32 *) &result);
        ASSERT_SIMPLE(false);
    }
}

void vulkan_sync_objects_create(VkDevice device, VkSemaphore* image_available_semaphore, VkSemaphore* render_finished_semaphore, VkFence* in_flight_fence)
{
    VkSemaphoreCreateInfo semaphore_info = {};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_info = {};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VkResult result;
    if ((result = vkCreateSemaphore(device, &semaphore_info, NULL, image_available_semaphore)) != VK_SUCCESS
        || (result = vkCreateSemaphore(device, &semaphore_info, NULL, render_finished_semaphore)) != VK_SUCCESS
        || (result = vkCreateFence(device, &fence_info, NULL, in_flight_fence)) != VK_SUCCESS
    ) {
        LOG_FORMAT(true, "Vulkan vulkan_sync_objects_create: %d", LOG_DATA_INT32, (int32 *) &result);

        ASSERT_SIMPLE(false);
    }
}

#endif