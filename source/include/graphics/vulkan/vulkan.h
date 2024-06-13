#include <X11/Xlib.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_xlib.h>
#include <vector>
#include "depth.h"
#include "mesh.h"
#include "texture.h"
#include "primatives/mesh.h"
#include "constants.h"
#include "buffer.h"
#include "interfaces/window.h"
#include "graphics/vulkan/interfaces/vulkan.h"
#include "graphics/vulkan/interfaces/frames.h"
#include "graphics/vulkan/interfaces/buffer.h"
#include "graphics/vulkan/interfaces/mesh.h"
#include "graphics/vulkan/shader/interfaces/shader.h"
#include "graphics/vulkan/stage.h"

#ifndef _VULKAN
#define _VULKAN

namespace vulkan
{
    class pipeline;

    class vulkan : public interfaces::vulkan
    {
        friend class frames::base;
        friend class depth;
        friend class texture;

    private:
        int width, height;

        VkPhysicalDevice vkPhysicalDevice;
        VkPhysicalDeviceProperties vkPhysicalDeviceProperties;

        VkInstance vkInstance;
        VkDevice vkDevice;
        VkSurfaceKHR vkSurface;
        VkQueue vkPresentQueue, vkGraphicsQueue;
        VkSwapchainKHR vkSwapChain;
        VkExtent2D vkExtent;

        VkSurfaceFormatKHR vkSurfaceFormat;
        VkPresentModeKHR vkPresentMode;

        VkFence vkRenderFence;

        std::vector<VkImageView> swapChainImageViews;
                
        std::vector<shader::interfaces::shader*> shaders;
        std::vector<interfaces::mesh*> meshes;
        std::vector<interfaces::buffer*> buffers;
        std::vector<pipeline*> pipelines;
        std::vector<stage*> stages;
        std::vector<interfaces::frames*> frame;
        std::vector<depth*> depths;
        std::vector<texture*> textures;        

        XSetWindowAttributes windowAttrib;
        interfaces::window *context;
        
        uint32_t queueFamilyIndex;
        uint32_t queuePresentIndex;

        bool init;

    public:
        vulkan(interfaces::window *source) { makeNull(); reset(source); }
        ~vulkan() { cleanup(); }

        void reset(interfaces::window *source);
        bool initalised() { return init; }

        template <std::derived_from<shader::interfaces::shader> T> shader::interfaces::shader *createShader(shader::parameters params)
        {
            shader::interfaces::shader *temp = new T();
            if(temp != NULL)
            {
                if(temp->create(vkDevice, params))
                {
                    shaders.push_back(temp);
                    return temp;
                }

                delete temp;
            }

            return NULL;
        }

        template <std::derived_from<interfaces::mesh> T> interfaces::mesh *createMesh(primatives::mesh vertices)
        {
            interfaces::mesh *temp = new T();
            if(temp != NULL)
            {
                if(temp->create(this, vertices))
                {
                    meshes.push_back(temp);
                    return temp;
                }

                delete temp;
            }

            return NULL;
        }

        texture *createTexture(uint32_t width, uint32_t height);
        
        template <std::derived_from<interfaces::buffer> T> interfaces::buffer *createBuffer(void *data, size_t size)
        {
           interfaces::buffer *temp = new T();
            if(temp != NULL)
            {
                if(temp->create(this, data, size))
                {
                    buffers.push_back(temp);
                    return temp;
                }

                delete temp;
            }

            return NULL;
        }

    public:
        depth *createDepthBuffer();

        template <std::derived_from<interfaces::frames> T> interfaces::frames *createFrames(depth *d) 
        { 
            interfaces::frames *temp = new T();
            if(temp != NULL)
            {
                if(temp->create(this, d))
                {
                    frame.push_back(temp);
                    return temp;
                }

                delete temp;
            }

            return NULL;
        } 

        stage *createStage(shader::interfaces::shader *vertex,
                            shader::interfaces::shader *fragment,
                            std::vector<interfaces::mesh*> meshes,
                            ::vulkan::constants *constants);        
                
    public:
        VkDevice getDevice() { return vkDevice; }
        VkExtent2D getExtent() { return vkExtent; }
        VkQueue getGraphicsQueue() { return vkGraphicsQueue; }
        VkQueue getPresentQueue() { return vkPresentQueue; }
        VkSwapchainKHR getSwapChain() { return vkSwapChain; }
        VkSurfaceFormatKHR getSurfaceFormat() { return vkSurfaceFormat; }
        uint32_t getQueueFamilyIndex() { return queueFamilyIndex; }
        std::vector<VkImageView> getSwapChainImageViews() { return swapChainImageViews; }

        size_t getMinUniformBufferOffsetAlignment() { return vkPhysicalDeviceProperties.limits.minUniformBufferOffsetAlignment; }
        size_t getMinStorageBufferOffsetAlignment() { return vkPhysicalDeviceProperties.limits.minStorageBufferOffsetAlignment; }

        float getMaxSamplerAnisotropy() { return vkPhysicalDeviceProperties.limits.maxSamplerAnisotropy; }

    public:    
        pipeline *createPipeline(interfaces::frames *frame,
                                 stage *stage);
        bool renderPipelines(std::vector<pipeline*> pipelines);

    protected:
        bool createInstance(bool enableLayer = true);
        bool findLayers(const std::vector<const char*> &validationLayers);
        VkPhysicalDevice findDevice(uint32_t index);
        bool createDevice(VkPhysicalDevice &device, uint32_t queueFamilyIndex, uint32_t queuePresentIndex);
        bool findQueueFamily(VkPhysicalDevice &device, uint32_t &queueFamilyIndex);
        bool findQueuePresentationFamily(VkPhysicalDevice &device, uint32_t &queuePresentIndex);
        bool createSurface(VkPhysicalDevice &device, uint32_t queue);
        bool findDeviceExtensionSupport(VkPhysicalDevice &device);
        bool createSwapChain(VkPhysicalDevice &device);
        bool createImageViews();
        bool createImageView(VkImageView &destination, VkImage image, VkFormat format);        
        bool createFences();

        VkExtent2D findSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    protected:
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);        

    protected:
        void makeNull();
        void cleanup();
    };
};

#endif