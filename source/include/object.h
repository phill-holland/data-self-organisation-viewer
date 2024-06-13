#include "graphics/vulkan/primatives/matrix.h"

#ifndef _ENGINE_OBJECT
#define _ENGINE_OBJECT

namespace engine
{
    using namespace vulkan;

    class object
	{
	public:
		primatives::matrices::matrix4x4 position;
		primatives::matrices::matrix4x4 matrix;
		primatives::vector3 colour;
		float padding;
	};
};

#endif