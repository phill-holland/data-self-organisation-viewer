#include <vulkan/vulkan.h>
#include "graphics/vulkan/interfaces/vulkan.h"
#include "graphics/vulkan/interfaces/buffer.h"

#ifndef _VULKAN_BUFFER
#define _VULKAN_BUFFER

namespace vulkan
{
    class vulkan;

    namespace buffer
    {
        class base
        {
        protected:
            size_t alignMemoryBuffer(size_t size, size_t minUboAlignment);
        };

        class uniformBuffer : public base, public interfaces::buffer
        {        
            VkBuffer vkBuffer;
            VkDeviceMemory vkBufferMemory;
            //VkDescriptorSet vkDescriptorSet;

            interfaces::vulkan *device;

            void *_data;
            int _length;

        private:
            friend class vulkan;

        public:
            uniformBuffer() { }

            bool create(interfaces::vulkan *device, void *data, size_t size);
            void destroy();        
            
        public:
            void update();

        public:
            void *getData() { return _data; }
            size_t getLength() { return _length; }

        public:
            VkBuffer getBuffer() { return vkBuffer; }
            //VkDescriptorSet *getDescriptorSet() { return &vkDescriptorSet; }
            VkDescriptorType getDescriptorType() { return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; }
            //type getType() { return type::global; }
        };

        class storageBuffer : public base, public interfaces::buffer
        {        
            VkBuffer vkBuffer;
            VkDeviceMemory vkBufferMemory;
            //VkDescriptorSet vkDescriptorSet;

            interfaces::vulkan *device;

            void *_data;
            int _length;

        private:
            friend class vulkan;

        public:
            storageBuffer() { }
        
            bool create(interfaces::vulkan *device, void *data, size_t size);
            void destroy();        
            
        public:
            void update();

        public:
            void *getData() { return _data; }
            size_t getLength() { return _length; }

        public:
            VkBuffer getBuffer() { return vkBuffer; }
            //VkDescriptorSet *getDescriptorSet() { return &vkDescriptorSet; }
            VkDescriptorType getDescriptorType() { return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER; }
            //type getType() { return type::object; }
        };

        class indirectBuffer : public base, public interfaces::buffer
        {        
            VkBuffer vkBuffer;
            VkDeviceMemory vkBufferMemory;
            //VkDescriptorSet vkDescriptorSet;

            interfaces::vulkan *device;

            void *_data;
            int _length;

        private:
            friend class vulkan;

        public:
            indirectBuffer() { }

            bool create(interfaces::vulkan *device, void *data, size_t size);
            void destroy();        
            
        public:
            void update();

        public:
            void *getData() { return _data; }
            size_t getLength() { return _length; }

        public:
            VkBuffer getBuffer() { return vkBuffer; }          
            //VkDescriptorSet *getDescriptorSet() { return &vkDescriptorSet; }
            VkDescriptorType getDescriptorType() { return VK_DESCRIPTOR_TYPE_MAX_ENUM; }
            //type getType() { return type::global; }
        };
    };
};

#endif