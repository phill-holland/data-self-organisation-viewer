#include <vulkan/vulkan.h>
#include "graphics/vulkan/primatives/mesh.h"
#include "graphics/vulkan/interfaces/vulkan.h"

#ifndef _VULKAN_INTERFACE_MESH
#define _VULKAN_INTERFACE_MESH

namespace vulkan
{
    class vulkan;

    namespace interfaces
    {
        class mesh
        {
        public:
            virtual ~mesh() { }
            
        public:
            virtual bool create(interfaces::vulkan *device, primatives::mesh vertices) = 0;
            virtual void destroy() = 0; 

        public:
            virtual VkBuffer getBuffer() = 0;

        public:
            virtual int getLength() = 0;
        };
    };
};

#endif