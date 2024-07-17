#include "graphics/vulkan/pipeline.h"
#include "graphics/vulkan/vulkan.h"
#include "graphics/vulkan/primatives/matrix.h"
#include "graphics/vulkan/buffer.h"
#include <stdio.h>

bool vulkan::pipeline::render()
{
    VkDevice vkDevice = device->getDevice();

    vkWaitForFences(vkDevice, 1, &vkRenderFence, VK_TRUE, UINT64_MAX);
    vkResetFences(vkDevice, 1, &vkRenderFence);

    uint32_t imageIndex;
    vkAcquireNextImageKHR(vkDevice, device->getSwapChain(), UINT64_MAX, vkImageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { vkImageAvailableSemaphore };
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = { vkRenderFinishedSemaphore };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(device->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) return false;

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { device->getSwapChain() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    vkQueuePresentKHR(device->getPresentQueue(), &presentInfo);

    return true;
}

bool vulkan::pipeline::create(interfaces::vulkan *device, 
                            interfaces::frames *frame, 
                            stage *stage)
{
    this->device = device;
    this->frame = frame;

    VkDevice vkDevice = device->getDevice();
    VkFormat vkFormat = device->getSurfaceFormat().format;
    VkExtent2D vkExtent = device->getExtent();

    if(!createPipelineLayout(vkDevice, stage)) return false;
    if(!createPipeline(vkDevice, vkExtent, stage->vertex, stage->fragment)) return false;
    if(!createCommandPool(vkDevice, device->getQueueFamilyIndex())) return false;
    if(!createCommandBuffers(vkDevice)) return false;
    if(!bindCommandQueue(stage)) return false;

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if(vkCreateSemaphore(vkDevice, &semaphoreInfo, nullptr, &vkImageAvailableSemaphore) != VK_SUCCESS) return false;
    if(vkCreateSemaphore(vkDevice, &semaphoreInfo, nullptr, &vkRenderFinishedSemaphore) != VK_SUCCESS) return false;
    
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if(vkCreateFence(vkDevice, &fenceInfo, nullptr, &vkRenderFence) != VK_SUCCESS) return false;

    return true;
}

void vulkan::pipeline::destroy()
{
    VkDevice vkDevice = device->getDevice();

    vkDestroyFence(vkDevice, vkRenderFence, nullptr);
    vkDestroySemaphore(vkDevice, vkRenderFinishedSemaphore, nullptr);
    vkDestroySemaphore(vkDevice, vkImageAvailableSemaphore, nullptr);

    vkDestroyCommandPool(vkDevice, vkCommandPool, nullptr);
    
    vkDestroyPipeline(vkDevice, vkGraphicsPipeline, nullptr);
    vkDestroyPipelineLayout(vkDevice, vkPipelineLayout, nullptr);
}

bool vulkan::pipeline::createPipelineLayout(VkDevice vkDevice, stage *stage)
{
    VkPushConstantRange push_constant;
    push_constant.offset = 0;
    push_constant.size = sizeof(::vulkan::constants);
    push_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;    

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        
    pipelineLayoutInfo.setLayoutCount = stage->vkDescriptorSetLayouts.size();
    pipelineLayoutInfo.pSetLayouts = stage->vkDescriptorSetLayouts.data();

    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    
    if(stage->constants != NULL)
    {
        pipelineLayoutInfo.pPushConstantRanges = &push_constant;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
    }
    
    if (vkCreatePipelineLayout(vkDevice, &pipelineLayoutInfo, nullptr, &vkPipelineLayout) != VK_SUCCESS) return false;

    return true;
}

bool vulkan::pipeline::createPipeline(VkDevice vkDevice, VkExtent2D vkExtent, shader::interfaces::shader *vertex, shader::interfaces::shader *fragment)
{
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;

    std::vector<VkPipelineShaderStageCreateInfo> shaderInfo;

    VkPipelineShaderStageCreateInfo shaderStageVertexInfo{};
    shaderStageVertexInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;    
    shaderStageVertexInfo.stage = vertex->getType();
    shaderStageVertexInfo.module = vertex->getShader();
    shaderStageVertexInfo.pName = vertex->getName().c_str();

    shaderInfo.push_back(shaderStageVertexInfo);

    shader::inputs vertexInputs = vertex->getInputs();
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &(vertexInputs.inputBindingDescription);    
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInputs.inputAttributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = vertexInputs.inputAttributeDescriptions.data();

    VkPipelineShaderStageCreateInfo shaderStageFragmentInfo{};
    shaderStageFragmentInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;    
    shaderStageFragmentInfo.stage = fragment->getType();
    shaderStageFragmentInfo.module = fragment->getShader();
    shaderStageFragmentInfo.pName = fragment->getName().c_str();

    shaderInfo.push_back(shaderStageFragmentInfo);

    VkPipelineShaderStageCreateInfo *shaderStages = &shaderInfo[0];

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) vkExtent.width;
    viewport.height = (float) vkExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = vkExtent;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = VK_FALSE;
    multisampling.alphaToOneEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.pNext = nullptr;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f;
    depthStencil.maxDepthBounds = 1.0f;
    depthStencil.stencilTestEnable = VK_FALSE;

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = shaderInfo.size();
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.layout = vkPipelineLayout;
    pipelineInfo.renderPass = frame->getRenderPass();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    if (vkCreateGraphicsPipelines(vkDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &vkGraphicsPipeline) != VK_SUCCESS) return false;

    return true;
}

bool vulkan::pipeline::createCommandPool(VkDevice vkDevice, uint32_t queueFamilyIndex)
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndex;
    poolInfo.flags = 0;

    if (vkCreateCommandPool(vkDevice, &poolInfo, nullptr, &vkCommandPool) != VK_SUCCESS) return false;

    return true;
}

bool vulkan::pipeline::createCommandBuffers(VkDevice vkDevice)
{
    commandBuffers.resize(frame->size());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = vkCommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    if (vkAllocateCommandBuffers(vkDevice, &allocInfo, commandBuffers.data()) != VK_SUCCESS) return false;

    return true;
}

bool vulkan::pipeline::indirectDraw(std::vector<interfaces::mesh*> meshes)                                    
{        
    indirectCommands.clear();

    int i = 0;
    for(std::vector<interfaces::mesh*>::iterator it = meshes.begin(); it < meshes.end(); ++it)
    {
        interfaces::mesh *temp = (*it);

        VkDrawIndirectCommand cmd {};
        cmd.vertexCount = temp->getLength();
        cmd.instanceCount = 2;
        cmd.firstVertex = 0;
        cmd.firstInstance = i;

        indirectCommands.push_back(cmd);

        ++i;
    }
        
    indirectCommandBuffer.create(device, indirectCommands.data(), meshes.size() * sizeof(VkDrawIndexedIndirectCommand));
    indirectCommandBuffer.update();
    
    return true;
}

bool vulkan::pipeline::update(int mesh_index, int instance_count, int first_instance)
{
    indirectCommands[mesh_index].instanceCount = instance_count;
    indirectCommands[mesh_index].firstInstance = first_instance;
    indirectCommandBuffer.update();

    return true;
}

bool vulkan::pipeline::bindCommandQueue(stage *stage)
{
    indirectDraw(stage->meshes);

    for (size_t i = 0; i < commandBuffers.size(); i++)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;

        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) return false;

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = frame->getRenderPass();
        renderPassInfo.framebuffer = frame->getFrameBuffer(i);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = device->getExtent();

        VkClearValue clearColour = {0.0f, 0.0f, 0.0f, 1.0f};        
        VkClearValue clearDepth;
        clearDepth.depthStencil = { 1.0f, 0 };

        VkClearValue clearValues[] = { clearColour, clearDepth };

        renderPassInfo.clearValueCount = 2;
        renderPassInfo.pClearValues = &clearValues[0];

        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vkGraphicsPipeline);
        
        stage->BindPipeline(commandBuffers[i], vkPipelineLayout);
        stage->Draw(commandBuffers[i], indirectCommandBuffer.getBuffer());    
                
        vkCmdEndRenderPass(commandBuffers[i]);

        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) return false;
    }

    return true;
}