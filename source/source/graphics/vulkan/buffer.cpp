#include "graphics/vulkan/buffer.h"
#include "graphics/vulkan/vulkan.h"
#include <string.h>

size_t vulkan::buffer::base::alignMemoryBuffer(size_t size, size_t minUboAlignment)
{            
    size_t alignedSize = size;
    if(minUboAlignment > 0)
        alignedSize = (alignedSize + minUboAlignment - 1) & ~(minUboAlignment - 1);

    return alignedSize;
}

bool vulkan::buffer::uniformBuffer::create(interfaces::vulkan *device, void *data, size_t size)
{
    _data = data;
    _length = size;

    this->device = device;

    VkDevice vkDevice = device->getDevice();

    VkBufferCreateInfo bufferInfo = {};

    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;    
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.pNext = nullptr;

    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

    if(vkCreateBuffer(vkDevice, &bufferInfo, nullptr, &vkBuffer) != VK_SUCCESS) return false;

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(vkDevice, vkBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};

    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = device->findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if(vkAllocateMemory(vkDevice, &allocInfo, nullptr, &vkBufferMemory) != VK_SUCCESS) return false;

    if(vkBindBufferMemory(vkDevice, vkBuffer, vkBufferMemory, 0) != VK_SUCCESS) return false;

    return true;
}

void vulkan::buffer::uniformBuffer::update()
{
    void *data_map;

    vkMapMemory(device->getDevice(), vkBufferMemory, 0, _length, 0, &data_map);
    memcpy(data_map, _data, (size_t)_length);
    vkUnmapMemory(device->getDevice(), vkBufferMemory);
}

void vulkan::buffer::uniformBuffer::destroy()
{
    VkDevice vkDevice = device->getDevice();
    
    vkDestroyBuffer(vkDevice, vkBuffer, nullptr);
    vkFreeMemory(vkDevice, vkBufferMemory, nullptr);
}

bool vulkan::buffer::storageBuffer::create(interfaces::vulkan *device, void *data, size_t size)
{
    _data = data;
    _length = size;

    this->device = device;

    VkDevice vkDevice = device->getDevice();

    VkBufferCreateInfo bufferInfo = {};

    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;    
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.pNext = nullptr;

    bufferInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;

    if(vkCreateBuffer(vkDevice, &bufferInfo, nullptr, &vkBuffer) != VK_SUCCESS) return false;

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(vkDevice, vkBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};

    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = device->findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if(vkAllocateMemory(vkDevice, &allocInfo, nullptr, &vkBufferMemory) != VK_SUCCESS) return false;

    if(vkBindBufferMemory(vkDevice, vkBuffer, vkBufferMemory, 0) != VK_SUCCESS) return false;

    return true;
}

void vulkan::buffer::storageBuffer::update()
{
    void *data_map;

    vkMapMemory(device->getDevice(), vkBufferMemory, 0, _length, 0, &data_map);
    memcpy(data_map, _data, (size_t)_length);
    vkUnmapMemory(device->getDevice(), vkBufferMemory);
}

void vulkan::buffer::storageBuffer::destroy()
{
    VkDevice vkDevice = device->getDevice();
    
    vkDestroyBuffer(vkDevice, vkBuffer, nullptr);
    vkFreeMemory(vkDevice, vkBufferMemory, nullptr);
}

bool vulkan::buffer::indirectBuffer::create(interfaces::vulkan *device, void *data, size_t size)
{
    _data = data;
    _length = size;

    this->device = device;

    VkDevice vkDevice = device->getDevice();

    VkBufferCreateInfo bufferInfo = {};

    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;    
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.pNext = nullptr;

    bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |  VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;

    if(vkCreateBuffer(vkDevice, &bufferInfo, nullptr, &vkBuffer) != VK_SUCCESS) return false;

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(vkDevice, vkBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};

    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = device->findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if(vkAllocateMemory(vkDevice, &allocInfo, nullptr, &vkBufferMemory) != VK_SUCCESS) return false;

    if(vkBindBufferMemory(vkDevice, vkBuffer, vkBufferMemory, 0) != VK_SUCCESS) return false;

    return true;
}

void vulkan::buffer::indirectBuffer::update()
{
    void *data_map;

    vkMapMemory(device->getDevice(), vkBufferMemory, 0, _length, 0, &data_map);
    memcpy(data_map, _data, (size_t)_length);
    vkUnmapMemory(device->getDevice(), vkBufferMemory);
}

void vulkan::buffer::indirectBuffer::destroy()
{
    VkDevice vkDevice = device->getDevice();
    
    vkDestroyBuffer(vkDevice, vkBuffer, nullptr);
    vkFreeMemory(vkDevice, vkBufferMemory, nullptr);
}