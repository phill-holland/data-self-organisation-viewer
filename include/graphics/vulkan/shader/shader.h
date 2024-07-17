#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include "graphics/vulkan/shader/interfaces/shader.h"
#include "graphics/vulkan/shader/parameters.h"

#ifndef _VULKAN_SHADER
#define _VULKAN_SHADER

namespace vulkan
{
    class vulkan;
    class pipeline;
        
    namespace shader
    {        
        class base : public interfaces::shader
        {   
            VkShaderModule vkShader;

        protected:
            parameters _params;

        private:
            friend class ::vulkan::vulkan;
            friend class ::vulkan::pipeline;

        protected:
            base() { }

            bool create(VkDevice vkDevice, parameters params);
            void destroy(VkDevice vkDevice)
            {
                vkDestroyShaderModule(vkDevice, vkShader, nullptr);
            }

        public:
            inputs getInputs() { return _params.vertexInputDescriptions; }
            std::string getName() { return _params.name; }

        public:
            VkShaderModule getShader() { return vkShader; }
        };

        class vertex : public base
        {
        public:
            vertex() : base() { }
            ~vertex() { }

        public:
            std::optional<int> getVertices()
            {
                return std::optional<int>(_params.vertices);
            }

        public:
            VkShaderStageFlagBits getType() 
            {
                return VK_SHADER_STAGE_VERTEX_BIT;
            }
        };        

        class fragment : public base
        {
        public:
            fragment() : base() { }
            ~fragment() { }
            
        public:
            std::optional<int> getVertices()
            {
                return {};
            }

        public:
            VkShaderStageFlagBits getType() 
            {
                return VK_SHADER_STAGE_FRAGMENT_BIT;
            }
        };        
    };
};

#endif