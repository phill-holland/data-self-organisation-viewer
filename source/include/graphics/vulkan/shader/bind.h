#include <vulkan/vulkan.h>

#ifndef _VULKAN_SHADER_BINDING
#define _VULKAN_SHADER_BINDING

namespace vulkan
{
    namespace shader
    {        
        class bind
        {
        public:
            int set;
            int binding;

        public:
            bind(int _set = 0, int _binding = 0) 
            { 
                set = _set;
                binding = _binding; 
            }
        };
    };
};

#endif