#include "graphics/vulkan/constants.h"
#include "graphics/vulkan/primatives/matrix.h"
#include "graphics/vulkan/vulkan.h"
#include "graphics/vulkan/interfaces/buffer.h"
#include "transformation.h"

#ifndef _ENGINE_CAMERA
#define _ENGINE_CAMERA

namespace engine
{
    namespace camera
    {
        class settings
        {
        public:
            float fov;
            float near, far;
            float ar;

        public:
            settings(int width, int height)
            {
                fov = 90.0f;
                near = 0.1, far = 100.0;
                ar = (((float)height) / ((float)width));
            }
        };

        class camera
        {
            transformation transform;        

            bool init;

        public:
            ::vulkan::interfaces::buffer *buffer;
            ::vulkan::constants constants;

        public:
            camera(vulkan::vulkan *_vulkan, settings _settings) { makeNull(); reset(_vulkan, _settings); }
            ~camera() { cleanup(); }
            
            void reset(vulkan::vulkan *_vulkan, settings _settings);
            bool initalised() { return init; }

            void set(transformation &source)
            {
                transform = source;
            }

            void position(float x, float y, float z)
            {
                transform.world = primatives::matrices::translation({x, y, z});
            }

            void projection(settings _settings)
            {
                constants.m = vulkan::primatives::matrices::projection(_settings.fov, _settings.ar, _settings.near, _settings.far);
            }

        protected:
            void makeNull();
            void cleanup();
        };
    };
};

#endif