#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include "vertex.h"

#ifndef _VULKAN_PRIMATIVE_MESH
#define _VULKAN_PRIMATIVE_MESH

namespace vulkan
{
    namespace primatives
    {        
        class mesh
        {
        public:
            std::vector<vertex> vertices;            

        public:
            bool load(std::string filename);
        };
    }
};

#endif