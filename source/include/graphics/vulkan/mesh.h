#include <vulkan/vulkan.h>
#include "primatives/mesh.h"
#include "interfaces/vertex.h"
#include "graphics/vulkan/interfaces/vulkan.h"
#include "graphics/vulkan/interfaces/mesh.h"
#include <string.h>

#ifndef _VULKAN_MESH
#define _VULKAN_MESH

namespace vulkan
{
    template <std::derived_from<interfaces::vertex> T> class mesh : public interfaces::mesh
    {
        VkBuffer vkVertexBuffer;
        VkDeviceMemory vkVertexBufferMemory;

        interfaces::vulkan *device;

        std::vector<T> vertices;

        int length;

    public:
        mesh() { }

        bool create(interfaces::vulkan *device, primatives::mesh vertices)
        {
            this->device = device;

            VkDevice vkDevice = device->getDevice();
            
            for(std::vector<primatives::vertex>::iterator it = vertices.vertices.begin(); it < vertices.vertices.end(); ++it)
            {
                T temp;
                interfaces::vertex *t = &temp;
                t->copy(*it);
                this->vertices.push_back(temp);
            }

            VkBufferCreateInfo bufferInfo{};

            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = sizeof(T) * this->vertices.size();
            bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            if(vkCreateBuffer(vkDevice, &bufferInfo, nullptr, &vkVertexBuffer) != VK_SUCCESS) return false;

            VkMemoryRequirements memRequirements;
            vkGetBufferMemoryRequirements(vkDevice, vkVertexBuffer, &memRequirements);

            VkMemoryAllocateInfo allocInfo{};

            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = device->findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

            if(vkAllocateMemory(vkDevice, &allocInfo, nullptr, &vkVertexBufferMemory) != VK_SUCCESS) return false;

            vkBindBufferMemory(vkDevice, vkVertexBuffer, vkVertexBufferMemory, 0);

            void *data;

            vkMapMemory(vkDevice, vkVertexBufferMemory, 0, bufferInfo.size, 0, &data);
            memcpy(data, this->vertices.data(), (size_t)bufferInfo.size);
            vkUnmapMemory(vkDevice, vkVertexBufferMemory);

            length = this->vertices.size();

            return true;
        }

        void destroy()
        {
            VkDevice vkDevice = device->getDevice();

            vkDestroyBuffer(vkDevice, vkVertexBuffer, nullptr);
            vkFreeMemory(vkDevice, vkVertexBufferMemory, nullptr);
        }

    public:
        VkBuffer getBuffer() { return vkVertexBuffer; }
        int getLength() { return length; }
    };
};

#endif