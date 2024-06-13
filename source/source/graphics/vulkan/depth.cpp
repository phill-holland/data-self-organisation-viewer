#include "graphics/vulkan/depth.h"
#include "graphics/vulkan/vulkan.h"
#include <stdio.h>

bool vulkan::depth::create(vulkan *device)
{
    this->device = device;    

    VkDevice vkDevice = device->vkDevice;
    VkExtent2D vkExtent = device->vkExtent;

    if(!createDepthImage(vkDevice, vkExtent)) return false;
    
    return true;
}

void vulkan::depth::destroy()
{
    VkDevice vkDevice = device->vkDevice;
    
    vkDestroyImageView(vkDevice, vkDepthImageView, nullptr);
    vkFreeMemory(vkDevice, vkDepthImageMemory, nullptr);
    vkDestroyImage(vkDevice, vkDepthImage, nullptr);    
}

bool vulkan::depth::createDepthImage(VkDevice vkDevice, VkExtent2D vkExtent)
{    
    VkExtent3D depthExtent = {
        vkExtent.width,
        vkExtent.height,
        1
    };

    VkImageCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.imageType = VK_IMAGE_TYPE_2D;
    
    createInfo.format = VK_FORMAT_D32_SFLOAT;
    createInfo.extent = depthExtent;

    createInfo.mipLevels = 1;
    createInfo.arrayLayers = 1;
    createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    createInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    if(vkCreateImage(vkDevice, &createInfo, nullptr, &vkDepthImage) != VK_SUCCESS) return false;

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(vkDevice, vkDepthImage, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = device->findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    
    if(vkAllocateMemory(vkDevice, &allocInfo, nullptr, &vkDepthImageMemory) != VK_SUCCESS) return false;
    vkBindImageMemory(vkDevice, vkDepthImage, vkDepthImageMemory, 0);

    VkImageViewCreateInfo createViewInfo = {};

    createViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createViewInfo.pNext = nullptr;
    createViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createViewInfo.image = vkDepthImage;
    createViewInfo.format = VK_FORMAT_D32_SFLOAT;
    createViewInfo.subresourceRange.baseMipLevel = 0;
    createViewInfo.subresourceRange.levelCount = 1;
    createViewInfo.subresourceRange.baseArrayLayer = 0;
    createViewInfo.subresourceRange.layerCount = 1;
    createViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    
    if(vkCreateImageView(vkDevice, &createViewInfo, nullptr, &vkDepthImageView) != VK_SUCCESS) return false;
    
    return true;
}
