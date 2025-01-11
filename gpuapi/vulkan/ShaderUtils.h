/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_GPUAPI_VULKAN_SHADER_UTILS_H
#define TOS_GPUAPI_VULKAN_SHADER_UTILS_H

#include <vulkan/vulkan.h>

#include "../../stdlib/Types.h"
#include "../../memory/RingMemory.h"
#include "../../log/Log.h"

inline uint32_t shader_get_uniform_location(VkWriteDescriptorSet* descriptor, VkDevice device, VkCommandBuffer commandBuffer, VkDescriptorSet descriptorSet, uint32_t binding, VkDescriptorType descriptorType)
{
    VkWriteDescriptorSet descriptorWrite = {};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = descriptorSet;
    descriptorWrite.dstBinding = binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = descriptorType;
    descriptorWrite.descriptorCount = 1;
}

inline void shader_set_value(VkDevice device, VkCommandBuffer commandBuffer, VkDescriptorSet descriptorSet, uint32_t binding, VkDescriptorType descriptorType, int32_t value)
{
    VkDescriptorBufferInfo bufferInfo = {};
    bufferInfo.buffer = {};  // You should have a buffer holding the value
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(value);

    VkWriteDescriptorSet descriptorWrite = {};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = descriptorSet;
    descriptorWrite.dstBinding = binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = descriptorType;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;

    vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, NULL);
}

VkShaderModule shader_make(VkDevice device, const char* source, int32 source_size)
{
    // Create shader module create info
    VkShaderModuleCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = source_size;
    create_info.pCode = (uint32 *) source;

    // Create shader module
    VkShaderModule shader_module;
    VkResult result = vkCreateShaderModule(device, &create_info, NULL, &shader_module);

    if (result != VK_SUCCESS) {
        LOG(true, "Failed to create shader module");
        ASSERT_SIMPLE(false);

        return VK_NULL_HANDLE;
    }

    return shader_module;
}

inline
void pipeline_use(VkCommandBuffer command_list, VkPipeline pipeline)
{
    vkCmdBindPipeline(command_list, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
}

#endif