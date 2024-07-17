#include <vulkan/vulkan.h>
#include "graphics/vulkan/primatives/vertex.h"

#ifndef _VULKAN_INTERFACE_VERTEX
#define _VULKAN_INTERFACE_VERTEX

namespace vulkan
{
    class vulkan;

    namespace interfaces
    {
        class vertex
        {
        public:
            virtual ~vertex() { }
            
        public:
            virtual void copy(primatives::vertex source) = 0;
        };

        class descriptor
        {
        public:
            virtual VkVertexInputBindingDescription getBindingDescription() = 0;
            virtual std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() = 0;            
        };
    };
};

#endif