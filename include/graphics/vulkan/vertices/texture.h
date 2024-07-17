#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include "graphics/vulkan/primatives/vector.h"
#include "graphics/vulkan/interfaces/vertex.h"

#ifndef _VULKAN_VERTICES_TEXTURE
#define _VULKAN_VERTICES_TEXTURE

namespace vulkan
{
    namespace vertices
    {
        namespace texture
        {
            using namespace primatives;

            class vertex : public interfaces::vertex
            {
            public:
                vector3 position;
                vector3 colour;
                vector2 uv;

            public:
                vertex(vector3 _position = { 0.0f, 0.0f, 0.0f } , vector3 _colour = { 1.0f, 1.0f ,1.0f } , vector2 _uv = { 0.0f, 0.0f, } )
                {                
                    position = _position;
                    colour = _colour;
                    uv = _uv;
                }

                void copy(primatives::vertex source)
                {
                    position = source.position;
                    colour = source.colour;
                    uv = source.uv;
                }
            };

            class descriptor : public interfaces::descriptor
            {
            public:    
                VkVertexInputBindingDescription getBindingDescription() 
                {
                    VkVertexInputBindingDescription bindingDescription {};

                    bindingDescription.binding = 0;
                    bindingDescription.stride = sizeof(::vulkan::vertices::texture::vertex);
                    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

                    return bindingDescription;
                }

                std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() 
                {
                    std::vector<VkVertexInputAttributeDescription> attributeDescription;

                    VkVertexInputAttributeDescription a;
                    
                    a.binding = 0;
                    a.location = 0;
                    a.format = VK_FORMAT_R32G32B32_SFLOAT;
                    a.offset = offsetof(::vulkan::vertices::texture::vertex, position);

                    VkVertexInputAttributeDescription b;

                    b.binding = 0;
                    b.location = 1;
                    b.format = VK_FORMAT_R32G32B32_SFLOAT;
                    b.offset = offsetof(::vulkan::vertices::texture::vertex, colour);

                    VkVertexInputAttributeDescription c;

                    c.binding = 0;
                    c.location = 2;
                    c.format = VK_FORMAT_R32G32_SFLOAT;
                    c.offset = offsetof(::vulkan::vertices::texture::vertex, uv);

                    attributeDescription.push_back(a);
                    attributeDescription.push_back(b);
                    attributeDescription.push_back(c);

                    return attributeDescription;
                }       
            };
        };
    };
};

#endif