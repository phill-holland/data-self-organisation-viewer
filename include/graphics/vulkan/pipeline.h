#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include "buffer.h"
#include "texture.h"
#include "constants.h"
#include "graphics/vulkan/interfaces/frames.h"
#include "graphics/vulkan/interfaces/vertex.h"
#include "graphics/vulkan/interfaces/mesh.h"
#include "graphics/vulkan/interfaces/vulkan.h"
#include "graphics/vulkan/interfaces/buffer.h"
#include "graphics/vulkan/shader/interfaces/shader.h"
#include "graphics/vulkan/stage.h"

#ifndef _VULKAN_PIPELINE
#define _VULKAN_PIPELINE

namespace vulkan
{
    class pipeline
    {
        VkPipelineLayout vkPipelineLayout;
        VkPipeline vkGraphicsPipeline;
        VkCommandPool vkCommandPool;
     
        std::vector<VkCommandBuffer> commandBuffers;

        VkSemaphore vkImageAvailableSemaphore;
        VkSemaphore vkRenderFinishedSemaphore;
        VkFence vkRenderFence;

        buffer::indirectBuffer indirectCommandBuffer;
        std::vector<VkDrawIndirectCommand> indirectCommands;

        interfaces::vulkan *device;
        interfaces::frames *frame;

    private:
        friend class vulkan;
        friend class texture;

    private:
        pipeline() { }

        bool create(interfaces::vulkan *device, 
                    interfaces::frames *frame,
                    stage *stage);

        void destroy();

    public:
        bool render();
        bool update(int mesh_index, int instance_count, int first_instance);

    protected:
        bool createPipelineLayout(VkDevice vkDevice, stage *stage);
        bool createPipeline(VkDevice vkDevice, VkExtent2D vkExtent, shader::interfaces::shader *vertex, shader::interfaces::shader *fragment);
        bool createCommandPool(VkDevice vkDevice, uint32_t queueFamilyIndex);
        bool createCommandBuffers(VkDevice vkDevice);
        bool indirectDraw(std::vector<interfaces::mesh*> meshes);
        bool bindCommandQueue(stage *stage);            
    };
};

#endif