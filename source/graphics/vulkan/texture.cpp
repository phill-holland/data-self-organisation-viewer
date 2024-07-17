#include "graphics/vulkan/texture.h"
#include "graphics/vulkan/vulkan.h"
#include "graphics/vulkan/pipeline.h"
#include "graphics/vulkan/buffer.h"

bool vulkan::texture::create(interfaces::vulkan *device, uint32_t width, uint32_t height)
{
    this->device = device;
    this->width = width;
    this->height = height;
    this->size = width * height * 4;

    VkDevice vkDevice = device->getDevice();

    VkBufferCreateInfo bufferInfo = {};

    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = this->size;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.pNext = nullptr;

    bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    if(vkCreateBuffer(vkDevice, &bufferInfo, nullptr, &vkStagingBuffer) != VK_SUCCESS) return false;

    VkMemoryRequirements stagingMemRequirements;
    vkGetBufferMemoryRequirements(vkDevice, vkStagingBuffer, &stagingMemRequirements);

    VkMemoryAllocateInfo stagingAllocInfo = {};

    stagingAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    stagingAllocInfo.allocationSize = stagingMemRequirements.size;
    stagingAllocInfo.memoryTypeIndex = device->findMemoryType(stagingMemRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if(vkAllocateMemory(vkDevice, &stagingAllocInfo, nullptr, &vkStagingBufferMemory) != VK_SUCCESS) return false;

    if(vkBindBufferMemory(vkDevice, vkStagingBuffer, vkStagingBufferMemory, 0) != VK_SUCCESS) return false;

    // ***

    VkImageCreateInfo imageCreateInfo {};

    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.extent.width = width;
    imageCreateInfo.extent.height = height;
    imageCreateInfo.extent.depth = 1;
    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.arrayLayers = 1;

    imageCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.flags = 0;

    if (vkCreateImage(vkDevice, &imageCreateInfo, nullptr, &vkImage) != VK_SUCCESS) return false;
    
    VkMemoryRequirements imageMemRequirements;
    vkGetImageMemoryRequirements(vkDevice, vkImage, &imageMemRequirements);

    VkMemoryAllocateInfo imageAllocInfo = {};

    imageAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    imageAllocInfo.allocationSize = imageMemRequirements.size;
    imageAllocInfo.memoryTypeIndex = device->findMemoryType(imageMemRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if(vkAllocateMemory(vkDevice, &imageAllocInfo, nullptr, &vkImageMemory) != VK_SUCCESS) return false;

    if(vkBindImageMemory(vkDevice, vkImage, vkImageMemory, 0) != VK_SUCCESS) return false;

    if(!createTextureImageView()) return false;
    if(!createTextureSampler()) return false;
    
    return true;
}

bool vulkan::texture::createTextureImageView()
{
    return device->createImageView(vkTextureImageView, vkImage, VK_FORMAT_R8G8B8A8_SRGB);
}

bool vulkan::texture::createTextureSampler()
{
    VkSamplerCreateInfo samplerInfo {};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;

    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = device->getMaxSamplerAnisotropy();//device->vkPhysicalDeviceProperties.limits.maxSamplerAnisotropy;

    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;

    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    if (vkCreateSampler(device->getDevice(), &samplerInfo, nullptr, &vkTextureSampler) != VK_SUCCESS) return false;

    return true;
}

VkCommandBuffer vulkan::texture::begin(pipeline *source)
{
    
    VkCommandBufferAllocateInfo allocInfo {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = source->vkCommandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device->getDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void vulkan::texture::end(pipeline *source, VkCommandBuffer commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(device->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(device->getGraphicsQueue());

    vkFreeCommandBuffers(device->getDevice(), source->vkCommandPool, 1, &commandBuffer);
}

void vulkan::texture::transition(pipeline *source, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkCommandBuffer commandBuffer = begin(source);

    VkImageMemoryBarrier barrier {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;

    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    barrier.image = vkImage;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = 0;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } 
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
    {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } 
    else return;

    vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    
    end(source, commandBuffer);
}

bool vulkan::texture::copy(graphics::texture *source)
{
    void *data;

    vkMapMemory(device->getDevice(), vkStagingBufferMemory, 0, size, 0, &data);
    
    bool result = source->into(data, size);
    
    vkUnmapMemory(device->getDevice(), vkStagingBufferMemory);

    return result;
}

void vulkan::texture::transfer(pipeline *source)
{
    transition(source, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    
    VkCommandBuffer commandBuffer = begin(source);

    VkBufferImageCopy region {};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = { width, height, 1 };

    vkCmdCopyBufferToImage(commandBuffer, vkStagingBuffer, vkImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
    
    end(source, commandBuffer);

    transition(source, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

void vulkan::texture::destroy()
{
    VkDevice vkDevice = device->getDevice();
        
    //transitionImageLayout(source, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyDescriptorPool(vkDevice, vkDescriptorPool, nullptr);

    vkDestroySampler(vkDevice, vkTextureSampler, nullptr);
    vkDestroyImageView(vkDevice, vkTextureImageView, nullptr);

    vkDestroyImage(vkDevice, vkImage, nullptr);
    vkFreeMemory(vkDevice, vkImageMemory, nullptr);

    vkDestroyBuffer(vkDevice, vkStagingBuffer, nullptr);
    vkFreeMemory(vkDevice, vkStagingBufferMemory, nullptr);
}