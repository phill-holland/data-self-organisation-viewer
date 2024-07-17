#include <vulkan/vulkan.h>
#include <string>
#include <vector>

#ifndef _VULKAN_DEPTH
#define _VULKAN_DEPTH

namespace vulkan
{
    class vulkan;
    class pipeline;

    namespace frames
    {
        class base;
    }

    class depth
    {
        VkImage vkDepthImage;
        VkImageView vkDepthImageView;
        VkDeviceMemory vkDepthImageMemory;

        vulkan *device;

    private:
        friend class vulkan;
        friend class pipeline;
        friend class frames::base;

    private:
        depth() { }

        bool create(vulkan *device);
        void destroy();    

    protected:
        bool createDepthImage(VkDevice vkDevice, VkExtent2D vkExtent);
    };
};

#endif