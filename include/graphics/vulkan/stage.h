#include "graphics/vulkan/interfaces/vertex.h"
#include "graphics/vulkan/interfaces/mesh.h"
#include "graphics/vulkan/interfaces/vulkan.h"
#include "graphics/vulkan/interfaces/buffer.h"
#include "graphics/vulkan/texture.h"
#include "graphics/vulkan/shader/interfaces/shader.h"
#include "graphics/vulkan/shader/bind.h"
#include "graphics/vulkan/constants.h"
#include <vector>
#include <unordered_map>

#ifndef _VULKAN_STAGE
#define _VULKAN_STAGE

namespace vulkan
{
    class pipeline;

    class stage
    {
        friend class pipeline;

        std::vector<VkDescriptorSetLayout> vkDescriptorSetLayouts;
        std::vector<VkDescriptorSet> vkDescriptorSets;

        VkDescriptorPool vkDescriptorPool;

        interfaces::vulkan *device;
        
    protected:
        shader::interfaces::shader *vertex;
        shader::interfaces::shader *fragment;

        std::vector<interfaces::mesh*> meshes;

        std::unordered_map<int,std::vector<std::tuple<int,interfaces::buffer*>>> buffers;
        std::unordered_map<int,std::vector<std::tuple<int,texture*>>> textures;

        ::vulkan::constants *constants;

    public:
        stage() { }

    public:
        bool create(interfaces::vulkan *device,
                    shader::interfaces::shader *vertex,
                    shader::interfaces::shader *fragment,
                    std::vector<interfaces::mesh*> meshes,
                    ::vulkan::constants *constants);

        void destroy();

    protected:
        bool createDescriptorPool();
        bool createDescriptorSets();
        bool createDescriptors();

    public:
        bool BindPipeline(VkCommandBuffer &vkCommandBuffer, VkPipelineLayout &vkPipelineLayout);
        bool Draw(VkCommandBuffer &vkCommandBuffer, VkBuffer vkIndirectBuffer);

    public:
        bool update()
        {            
                if(!createDescriptorPool()) return false;
                if(!createDescriptorSets()) return false;
                if(!createDescriptors()) return false;
            
                return true;
        }

        bool push_back(interfaces::buffer* source, shader::bind bindings);
        bool push_back(texture* source, shader::bind bindings);

    protected:
        size_t max();
    };
};

#endif