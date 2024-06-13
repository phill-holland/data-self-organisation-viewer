#include "pipeline.h"
#include "graphics/vulkan/primatives/vertex.h"
#include "graphics/vulkan/mesh.h"
#include "graphics/vulkan/vertices/colour.h"
#include "graphics/vulkan/vertices/texture.h"
#include "graphics/vulkan/pipeline.h"
#include <iostream>
#include <thread>

void engine::pipeline::pipeline::reset(vulkan::vulkan *_vulkan, vulkan::interfaces::frames *_frames, camera::camera *_camera, vulkan::interfaces::descriptor *_descriptor, vulkan::texture *_texture, particles *_particles, int _type, settings _settings)
{
    init = false; cleanup();

	type = _type;

    shader::parameters params(std::string(_settings.vertex));

	params.vertexInputDescriptions.inputBindingDescription = _descriptor->getBindingDescription();
	params.vertexInputDescriptions.inputAttributeDescriptions = _descriptor->getAttributeDescriptions();

	vertex = _vulkan->createShader<shader::vertex>(params);    
	if(vertex == NULL) return;
	
	fragment = _vulkan->createShader<shader::fragment>(shader::parameters(std::string(_settings.fragment)));
	if(fragment == NULL) return;

	std::vector<interfaces::mesh*> meshes;

	primatives::mesh _square;
	if(!_square.load(_settings.model)) return;

	interfaces::mesh *square = NULL;
	if(dynamic_cast<vulkan::vertices::colour::descriptor*>(_descriptor))
	{
		square = _vulkan->createMesh<vulkan::mesh<vulkan::vertices::colour::vertex>>(_square);
	}
	else if (dynamic_cast<vulkan::vertices::texture::descriptor*>(_descriptor))
	{	
		square = _vulkan->createMesh<vulkan::mesh<vulkan::vertices::texture::vertex>>(_square);
	}

	if(square == NULL) return;

	meshes.push_back(square);

    for(std::vector<particle>::iterator it = _particles->values.begin(); it < _particles->values.end(); ++it)
    {
		if(it->type == _type)
		{
			object temp;

			temp.position = primatives::matrices::translation({0.0f,0.0f,0.0f});
			temp.matrix.identity();

			matrices.push_back(temp);
		}
    }

	if(matrices.size() == 0) return;
	
	storage = _vulkan->createBuffer<buffer::storageBuffer>(matrices.data(), matrices.size() * sizeof(object));


	stage = _vulkan->createStage(vertex, fragment, meshes, &_camera->constants);
	if(stage == NULL) return;
	
	stage->push_back(_camera->buffer, vulkan::shader::bind(0,0));
	stage->push_back(storage, vulkan::shader::bind(1,0));	
	stage->push_back(_texture, vulkan::shader::bind(2,0));

	_camera->buffer->update();
	storage->update();
	
	stage->update(); // rename to bind

	pipe = _vulkan->createPipeline(_frames, stage);
	if(pipe == NULL) return;

    init = true;
}

void engine::pipeline::pipeline::update(particles *_particles)
{
//std::this_thread::sleep_for(std::chrono::milliseconds(1));

	int count = 0;
	for(std::vector<particle>::iterator it = _particles->values.begin(); it < _particles->values.end(); ++it)
    {
		if((it->visible)&&(it->type == type))
		{
			matrices[count].matrix = primatives::matrices::translation({it->x,it->y,it->z});
			matrices[count].colour = primatives::vector3(it->red,it->green,it->blue);
			it->update();
			++count;
		}
	}

	pipe->update(0, count, 0);
	storage->update();
}

void engine::pipeline::pipeline::render()
{
    pipe->render();
}

void engine::pipeline::pipeline::makeNull()
{
    vertex = NULL;
    fragment = NULL;
    //buffer = NULL;
    storage = NULL;
    pipe = NULL;
	stage = NULL;
}

void engine::pipeline::pipeline::cleanup()
{
    // should be managed by vulkan class!
    //if(pipe != NULL) delete pipe;
    //if(storage != NULL) delete storage;
    //if(buffer != NULL) delete buffer;
    //if(fragment != NULL) delete fragment;
    //if(vertex != NULL) delete vertex;
}
