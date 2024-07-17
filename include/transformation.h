#include "graphics/vulkan/primatives/matrix.h"

#ifndef _ENGINE_TRANSFORMATION
#define _ENGINE_TRANSFORMATION

namespace engine
{
    using namespace vulkan;

	class transformation
	{
	public:
		primatives::matrices::matrix4x4 world;
		primatives::matrices::matrix4x4 object;
	};
};

#endif