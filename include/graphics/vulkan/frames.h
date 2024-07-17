#include "depth.h"
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include "graphics/vulkan/interfaces/frames.h"
#include "graphics/vulkan/interfaces/vulkan.h"

#ifndef _VULKAN_FRAMES
#define _VULKAN_FRAMES

namespace vulkan
{
    class vulkan;
    class pipeline;

    namespace frames
    {
        class base : public interfaces::frames
        {
            friend class vulkan;
            friend class pipeline;

        protected:
            VkRenderPass vkRenderPass;

            std::vector<VkFramebuffer> swapChainFramebuffers;

            interfaces::vulkan *device;
            depth *_depth;
            
        protected:
            base() { }            

            bool create(interfaces::vulkan *device, depth *d);
            void destroy();    

        public:
            VkRenderPass getRenderPass() { return vkRenderPass; }
            VkFramebuffer getFrameBuffer(uint32_t index) { return swapChainFramebuffers[index]; }
            size_t size() { return swapChainFramebuffers.size(); }

        protected:
            virtual bool createRenderPass(VkDevice vkDevice, VkFormat vkFormat) = 0;
            bool createFrameBuffers(VkDevice vkDevice, VkExtent2D vkExtent, std::vector<VkImageView> swapChainImageViews);
        };

        class framesClearDepthBuffer : public base
        {
        public:
            framesClearDepthBuffer() : base() { }
            ~framesClearDepthBuffer() { }

        protected:
            bool createRenderPass(VkDevice vkDevice, VkFormat vkFormat);
        };

        class framesReuseDepthBuffer : public base
        {
        public:
            framesReuseDepthBuffer() : base() { }
            ~framesReuseDepthBuffer() { }

        protected:
            bool createRenderPass(VkDevice vkDevice, VkFormat vkFormat);
        };
    };
};

#endif