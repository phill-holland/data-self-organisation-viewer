#include "camera.h"
#include "graphics/vulkan/buffer.h"

void engine::camera::camera::reset(vulkan::vulkan *_vulkan, settings _settings)
{
    init = false; cleanup();

    constants.m = vulkan::primatives::matrices::projection(_settings.fov, _settings.ar, _settings.near, _settings.far);

	transform.world = primatives::matrices::translation({0.8f,-0.8f,-3.8f});
	transform.object.identity();

	buffer = _vulkan->createBuffer<buffer::uniformBuffer>(&transform, sizeof(transformation));
    if(buffer == NULL) return;

    init = true;
}

void engine::camera::camera::makeNull()
{
    buffer = NULL;
}

void engine::camera::camera::cleanup()
{
}
