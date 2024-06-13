#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include "graphics/vulkan/shader/inputs.h"

#ifndef _VULKAN_SHADER_PARAMETERS
#define _VULKAN_SHADER_PARAMETERS

namespace vulkan
{
    namespace shader
    {        
        class parameters
        {
        public:            
            std::string filename;
            std::string name;
            int vertices;

        public:
            inputs vertexInputDescriptions;

        public:
            parameters() { }
            parameters(std::string _filename, int _vertices = 0, std::string _name = "main")
            {
                filename = _filename;
                vertices = _vertices;
                name = _name;
            }
        };
    };
};

#endif