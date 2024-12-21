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
    descriptorWrite.pBufferInfo->buffer = 0;
    descriptorWrite.pBufferInfo->offset = 0;
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

    vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
}

VkShaderModule shader_make(VkDevice device, const char* source, int32 source_size, RingMemory* ring)
{
    // Create shader module create info
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = source_size;
    createInfo.pCode = (uint32 *) source;

    // Create shader module
    VkShaderModule shaderModule;
    VkResult result = vkCreateShaderModule(device, &createInfo, NULL, &shaderModule);

    if (result != VK_SUCCESS) {
        LOG("Failed to create shader module", true, true);
        ASSERT_SIMPLE(false);

        return VK_NULL_HANDLE;
    }

    return shaderModule;
}

VkPipeline program_make(
    VkDevice device,
    VkShaderModule vertex_shader,
    VkShaderModule fragment_shader,
    VkShaderModule geometry_shader,
    VkPipelineLayout pipeline_layout,
    VkRenderPass render_pass,
    RingMemory* ring
) {
    VkPipelineShaderStageCreateInfo shaderStages[3];

    // Vertex shader
    shaderStages[0] = {};
    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module = vertex_shader;
    shaderStages[0].pName = "main";

    // Fragment shader
    shaderStages[1] = {};
    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].module = fragment_shader;
    shaderStages[1].pName = "main";

    VkPipelineShaderStageCreateInfo* geometryShaderStage = NULL;

    // Optional Geometry shader
    if (geometry_shader != VK_NULL_HANDLE) {
        geometryShaderStage = &shaderStages[2];
        geometryShaderStage->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        geometryShaderStage->stage = VK_SHADER_STAGE_GEOMETRY_BIT;
        geometryShaderStage->module = geometry_shader;
        geometryShaderStage->pName = "main";
    }

    // Create the pipeline
    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.stageCount = (geometry_shader != VK_NULL_HANDLE) ? 3 : 2;
    pipelineCreateInfo.pStages = shaderStages;
    pipelineCreateInfo.pInputAssemblyState = NULL;  // Define if needed
    pipelineCreateInfo.pVertexInputState = NULL;    // Define if needed
    pipelineCreateInfo.pViewportState = NULL;       // Define if needed
    pipelineCreateInfo.pRasterizationState = NULL;  // Define if needed
    pipelineCreateInfo.pMultisampleState = NULL;    // Define if needed
    pipelineCreateInfo.pDepthStencilState = NULL;   // Define if needed
    pipelineCreateInfo.pColorBlendState = NULL;     // Define if needed
    pipelineCreateInfo.pDynamicState = NULL;        // Define if needed
    pipelineCreateInfo.layout = pipeline_layout;
    pipelineCreateInfo.renderPass = render_pass;
    pipelineCreateInfo.subpass = 0;  // Assumes 0 subpass
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;

    VkPipeline graphicsPipeline;
    VkResult result = vkCreateGraphicsPipeline(device, VK_NULL_HANDLE, &pipelineCreateInfo, NULL, &graphicsPipeline);

    if (result != VK_SUCCESS) {
        LOG("Failed to create program", true, true);
        return VK_NULL_HANDLE;
    }

    return graphicsPipeline;
}

inline
void pipeline_use(VkCommandBuffer command_list, VkPipeline pipeline)
{
    vkCmdBindPipeline(command_list, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
}

#endif