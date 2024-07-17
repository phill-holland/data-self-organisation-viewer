#include <vulkan/vulkan.h>
#include "graphics/vulkan/depth.h"
#include "graphics/vulkan/interfaces/vulkan.h"

#ifndef _VULKAN_INTERFACE_FRAMES
#define _VULKAN_INTERFACE_FRAMES

namespace vulkan
{
    class vulkan;

    namespace interfaces
    {
        class frames
        {
        public:
            virtual ~frames() { }
            
        public:
            virtual bool create(interfaces::vulkan *device, depth *d) = 0;
            virtual void destroy() = 0; 

        public:
            virtual VkRenderPass getRenderPass() = 0;
            virtual VkFramebuffer getFrameBuffer(uint32_t index) = 0;            
            virtual size_t size() = 0;
        };
    };
};

#endif