#include <chrono>
#include <thread>
#include "graphics/vulkan/vulkan.h"
#include "graphics/vulkan/primatives/vertex.h"
#include "graphics/vulkan/primatives/mesh.h"
#include "graphics/vulkan/primatives/matrix.h"
#include "graphics/vulkan/buffer.h"
#include "graphics/vulkan/constants.h"
#include "graphics/vulkan/frames.h"
#include "graphics/vulkan/depth.h"
#include "graphics/window.h"
#include "tracer/tracer.h"
#include "engine.h"
#include <random>
#include <iostream>

using namespace vulkan;

int main(int argc, char *argv[])
{
	tracer::tracer data;

	if(data.load("assets/data/trace_triple5.txt"))
	{
		engine::engine e(engine::settings(800, 800), data);
		e.run();
	}

	return 0;
}