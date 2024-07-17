#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include "graphics/vulkan/primatives/vector.h"

#ifndef _VULKAN_PRIMATIVE_VERTEX
#define _VULKAN_PRIMATIVE_VERTEX

namespace vulkan
{
    namespace primatives
    {       
        class vertex
        {
        public:
            vector3 position;
            vector3 colour;
            vector2 uv;
            vector3 normal;

        public:
            vertex(vector3 _position = { 0.0f, 0.0f, 0.0f } , vector3 _colour = { 1.0f, 1.0f ,1.0f } , vector2 _uv = { 0.0f, 0.0f }, vector3 _normal = { 1.0f, 0.0f, 0.0f } )
            {                
                position = _position;
                colour = _colour;
                uv = _uv;
                normal = _normal;
            }
        };
    };
};

#endif