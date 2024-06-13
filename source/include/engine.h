#include "graphics/vulkan/vulkan.h"
#include "graphics/vulkan/interfaces/frames.h"
#include "graphics/vulkan/depth.h"
#include "graphics/window.h"
#include "core/thread.h"
#include "tracer/tracer.h"
#include "camera.h"
#include "pipeline.h"
#include "particles.h"
#include <random>
#include <chrono>

#ifndef _ENGINE_ENGINE
#define _ENGINE_ENGINE

namespace engine
{
    class settings
    {
    public:
        int width, height;

    public:
        settings(int _width = 800, int _height = 600) 
        {
            width = _width;
            height = _height;
        }
    };

    class engine : public core::thread
    {
        static std::mt19937_64 generator;
        static const int total = 100;
        
        std::chrono::high_resolution_clock::time_point previous;
        double accumlator;

        vulkan::vulkan *vulkan;
        vulkan::window *window;
        camera::camera *camera; 

        ::engine::particles *particles;

        vulkan::depth *depth;
        //vulkan::texture *texture;
        vulkan::interfaces::frames *clearDepthFrames, *reuseDepthFrames;
        pipeline::pipeline *a;//, *b, *c;

        tracer::tracer data;

        int currentFrame;

        bool init;

	protected:
		void background(core::thread *bt);

    public:
        engine(settings _settings, tracer::tracer data) { makeNull(); reset(_settings, data); }
        ~engine() { cleanup(); }

        void reset(settings _settings, tracer::tracer data);
        bool initalised() { return init; }

        void run();

    protected:
        void makeNull();
        void cleanup();
    };
};

#endif