#include <vulkan/vulkan.h>
#include <vector>

#ifndef _VULKAN_INTERFACE_VULKAN
#define _VULKAN_INTERFACE_VULKAN

namespace vulkan
{    
    namespace interfaces
    {
        class vulkan
        {
        public:
            virtual ~vulkan() { }
            
        public:
            virtual VkDevice getDevice() = 0;
            virtual VkExtent2D getExtent() = 0;
            virtual VkQueue getGraphicsQueue() = 0;
            virtual VkQueue getPresentQueue() = 0;
            virtual VkSwapchainKHR getSwapChain() = 0;
            virtual VkSurfaceFormatKHR getSurfaceFormat() = 0;
            virtual uint32_t getQueueFamilyIndex() = 0;
            virtual std::vector<VkImageView> getSwapChainImageViews() = 0;
            
            virtual size_t getMinUniformBufferOffsetAlignment() = 0;
            virtual size_t getMinStorageBufferOffsetAlignment() = 0;

            virtual float getMaxSamplerAnisotropy() = 0;

            // ***
            virtual bool createImageView(VkImageView &destination, VkImage image, VkFormat format) = 0;
            // ***

            virtual uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) = 0;
        };
    };
};

#endif