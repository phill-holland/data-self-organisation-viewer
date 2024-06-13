#include <vulkan/vulkan.h>
#include "graphics/texture.h"
#include "graphics/vulkan/interfaces/buffer.h"
#include "graphics/vulkan/interfaces/vulkan.h"

#ifndef _VULKAN_TEXTURE
#define _VULKAN_TEXTURE

namespace vulkan
{
    //class vulkan;
    class pipeline;
    //class buffer;

    class texture
    {        
        VkBuffer vkStagingBuffer;
        VkDeviceMemory vkStagingBufferMemory;

        VkImage vkImage;
        VkDeviceMemory vkImageMemory;
    
        //VkDescriptorSet vkDescriptorSet;

        //VkDescriptorSetLayout vkSamplerSetLayout;
        
        VkDescriptorPool vkDescriptorPool;

        uint32_t width, height;
        size_t size;

        interfaces::vulkan *device;

    public:
        VkImageView vkTextureImageView;
        VkSampler vkTextureSampler;

    private:
        //friend class vulkan;
        friend class pipeline;

    public:
        texture() { }

        bool create(interfaces::vulkan *device, uint32_t width, uint32_t height);
        void destroy();        
        
        bool createTextureImageView();
        bool createTextureSampler();

        //VkDescriptorSet *getDescriptorSet() { return &vkDescriptorSet; }

    protected:
        VkCommandBuffer begin(pipeline *source);
        void end(pipeline *source, VkCommandBuffer commandBuffer);
        void transition(pipeline *source, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    public:
        bool copy(graphics::texture *source);

    public:
        void transfer(pipeline *source);
    };
};

#endif