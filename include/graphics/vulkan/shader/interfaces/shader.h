#include <vulkan/vulkan.h>
#include <string>
#include <optional>
#include "graphics/vulkan/shader/parameters.h"
#include "graphics/vulkan/shader/inputs.h"
#include "graphics/vulkan/interfaces/vulkan.h"

#ifndef _VULKAN_SHADER_INTERFACE_SHADER
#define _VULKAN_SHADER_INTERFACE_SHADER

namespace vulkan
{
    class vulkan;

    namespace shader
    {
        namespace interfaces
        {
            class shader
            {
            public:
                virtual ~shader() { }
                
            public:
                virtual bool create(VkDevice vkDevice, parameters params) = 0;
                virtual void destroy(VkDevice vkDevice) = 0;
                
            public:
                virtual inputs getInputs() = 0;
                virtual std::string getName() = 0;
                virtual std::optional<int> getVertices() = 0;

            public:
                virtual VkShaderModule getShader() = 0;
                virtual VkShaderStageFlagBits getType() = 0;            
            };
        };
    };
};

#endif