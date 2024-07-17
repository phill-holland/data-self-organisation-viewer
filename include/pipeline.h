#include "graphics/vulkan/vulkan.h"
#include "graphics/vulkan/shader/shader.h"
#include "graphics/vulkan/texture.h"
#include "graphics/vulkan/stage.h"
#include "graphics/vulkan/interfaces/frames.h"
#include "graphics/vulkan/interfaces/buffer.h"
#include "graphics/vulkan/interfaces/vertex.h"
#include "object.h"
#include "particle.h"
#include "particles.h"
#include "transformation.h"
#include "camera.h"
#include <string>
#include <chrono>

#ifndef _ENGINE_PIPELINE
#define _ENGINE_PIPELINE

namespace engine
{
    namespace pipeline
    {
        using namespace vulkan;

        class settings
        {
        public:        
            std::string fragment;
            std::string vertex;
            std::string model;

        public:
            settings(std::string fragment = "", std::string vertex = "", std::string model = "") 
            { 
                this->fragment = fragment;
                this->vertex = vertex;
                this->model = model;
            }
        };

        class pipeline
        {    
            shader::interfaces::shader *vertex, *fragment;
            vulkan::interfaces::buffer *storage;

            std::vector<object> matrices;

            int type;

            bool init;

        public:
            vulkan::pipeline *pipe;
            vulkan::stage *stage;

        public:
            pipeline(vulkan::vulkan *_vulkan, 
                    vulkan::interfaces::frames *_frames,
                    camera::camera *_camera,
                    vulkan::interfaces::descriptor *_descriptor,
                    vulkan::texture *_texture,
                    particles *_particles,
                    int _type,
                    settings _settings) 
            { 
                makeNull(); 
                reset(_vulkan, _frames, _camera, _descriptor, _texture, _particles, _type, _settings); 
            }
            ~pipeline() { cleanup(); }

            void reset(vulkan::vulkan *_vulkan, 
                       vulkan::interfaces::frames *_frames,
                       camera::camera *_camera,
                       vulkan::interfaces::descriptor *_descriptor,
                       vulkan::texture *_texture,
                       particles *_particles,
                       int _type,
                       settings _settings);

            bool initalised() { return init; }

            void update(particles *_particles);
            void render();

        protected:
            void makeNull();
            void cleanup();
        };
    };
};

#endif