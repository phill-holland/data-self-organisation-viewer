#include "graphics/vulkan/shader/shader.h"
#include "graphics/vulkan/pipeline.h"
#include <iostream>
#include <fstream>
#include <vector>

bool vulkan::shader::base::create(VkDevice vkDevice, parameters params)
{
    _params = params;
    
    std::ifstream file(_params.filename, std::ios::ate | std::ios::binary);

    if (file.is_open())
    {
        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = buffer.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());

        if (vkCreateShaderModule(vkDevice, &createInfo, nullptr, &vkShader) == VK_SUCCESS) return true;
    }

    return false;
}