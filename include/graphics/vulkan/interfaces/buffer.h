#include <vulkan/vulkan.h>
#include "graphics/vulkan/depth.h"
#include "graphics/vulkan/interfaces/vulkan.h"

#ifndef _VULKAN_INTERFACE_BUFFER
#define _VULKAN_INTERFACE_BUFFER

namespace vulkan
{
    class vulkan;

    namespace interfaces
    {
        class buffer
        {
        /*public:
            enum type
            {
                global = 0,
                object = 1
            };
        */
        public:
            virtual ~buffer() { }
            
        public:
            virtual bool create(interfaces::vulkan *device, void *data, size_t size) = 0;
            virtual void destroy() = 0; 

        public:
            virtual void update() = 0;

        public:
            virtual void *getData() = 0;
            virtual size_t getLength() = 0;

        public:
            virtual VkBuffer getBuffer() = 0;
            //virtual VkDescriptorSet *getDescriptorSet() = 0;
            virtual VkDescriptorType getDescriptorType() = 0;
            //virtual type getType() = 0;
        };
    };
};

#endif
