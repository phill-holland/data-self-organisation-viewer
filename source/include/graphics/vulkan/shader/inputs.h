#include <vulkan/vulkan.h>
#include <string>
#include <vector>

#ifndef _VULKAN_SHADER_INPUTS
#define _VULKAN_SHADER_INPUTS

namespace vulkan
{
    namespace shader
    {        
        class inputs
        {
        public:
            VkVertexInputBindingDescription inputBindingDescription;
            std::vector<VkVertexInputAttributeDescription> inputAttributeDescriptions;

        public:
            bool is_empty() { return inputAttributeDescriptions.size() == 0; }
        };
    };
};

#endif