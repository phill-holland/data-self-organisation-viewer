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
#include "engine.h"
#include <random>
#include <iostream>

using namespace vulkan;
/*
int basicVertexAndFragmentShaders()
{
	const int width = 800, height = 800;

	vulkan::window w(width, height);
	vulkan::vulkan v(&w);
	
	shader::shader *vertex = v.createShader(shader::parameters(std::string("assets/shaders/compiled/basic.vert.spv"), shader::TYPE::vertex, 3 ));
	if(vertex == NULL) return 0;

	shader::shader *fragment = v.createShader(shader::parameters(std::string("assets/shaders/compiled/shader.frag.spv"), shader::TYPE::fragment));
	if(fragment == NULL) return 0;

	std::vector<shader::shader*> shaders;

	shaders.push_back(vertex);
	shaders.push_back(fragment);

	std::vector<mesh*> meshes;
	std::vector<buffer*> buffers;

	pipeline *pipeline = v.createPipeline(shaders, meshes, buffers, NULL);
	if(pipeline == NULL) return 0;

	while(true)
	{
		pipeline->render();
	}

	return 0;
}

int basicMeshShaders()
{
	const int width = 800, height = 800;

	vulkan::window w(width, height);
	vulkan::vulkan v(&w);
	
	shader::parameters params(std::string("assets/shaders/compiled/basic.vert.spv"), shader::TYPE::vertex);
	params.vertexInputDescriptions.inputBindingDescription = primatives::vertex::getBindingDescription();
	params.vertexInputDescriptions.inputAttributeDescriptions = primatives::vertex::getAttributeDescriptions();

	shader::shader *vertex = v.createShader(params);
	if(vertex == NULL) return 0;
	
	shader::shader *fragment = v.createShader(shader::parameters(std::string("assets/shaders/compiled/shader.frag.spv"), shader::TYPE::fragment));
	if(fragment == NULL) return 0;

	std::vector<shader::shader*> shaders;

	shaders.push_back(vertex);
	shaders.push_back(fragment);

	std::vector<mesh*> meshes;

	primatives::mesh pmesh;
	pmesh.vertices = {{{0.0f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}},
    				  {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    				  {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}};

	mesh *triangle = v.createMesh(pmesh);
	if(triangle == NULL) return 0;

	meshes.push_back(triangle);

	std::vector<buffer*> buffers;

	pipeline *pipeline = v.createPipeline(shaders, meshes, buffers, NULL);
	if(pipeline == NULL) return 0;

	while(true)
	{
		pipeline->render();
	}

	return 0;
}

int basicLoadObjMeshShaders()
{
	primatives::mesh pmesh;
	if(!pmesh.load("assets/meshes/triangle.obj")) return 0;

	const int width = 800, height = 800;

	vulkan::window w(width, height);
	vulkan::vulkan v(&w);
	
	shader::parameters params(std::string("assets/shaders/compiled/mesh.vert.spv"), shader::TYPE::vertex);
	params.vertexInputDescriptions.inputBindingDescription = primatives::vertex::getBindingDescription();
	params.vertexInputDescriptions.inputAttributeDescriptions = primatives::vertex::getAttributeDescriptions();

	shader::shader *vertex = v.createShader(params);
	if(vertex == NULL) return 0;
	
	shader::shader *fragment = v.createShader(shader::parameters(std::string("assets/shaders/compiled/shader.frag.spv"), shader::TYPE::fragment));
	if(fragment == NULL) return 0;

	std::vector<shader::shader*> shaders;

	shaders.push_back(vertex);
	shaders.push_back(fragment);

	std::vector<mesh*> meshes;

	mesh *triangle = v.createMesh(pmesh);
	if(triangle == NULL) return 0;

	meshes.push_back(triangle);

	std::vector<buffer*> buffers;

	primatives::matrices::translation a({-0.3f,0.3f,0.0f});

	vulkan::buffer *buffer = v.createBuffer(&a, sizeof(primatives::matrices::translation));
	buffers.push_back(buffer);

	::vulkan::constants constants;
	constants.m = primatives::matrices::translation({0.3f,0.3f,0.0f});

	pipeline *pipeline = v.createPipeline(shaders, meshes, buffers, &constants);
	if(pipeline == NULL) return 0;

	float offset = -0.3f;
	while(true)
	{
		pipeline->render();

		a.x(offset);
		offset += 0.0001f;
		buffer->update();
	}

	return 0;
}

int basicMeshProjection()
{
	const int width = 800, height = 800;

	vulkan::window w(width, height);
	vulkan::vulkan v(&w);
	
	shader::parameters params(std::string("assets/shaders/compiled/projection.vert.spv"), shader::TYPE::vertex);
	params.vertexInputDescriptions.inputBindingDescription = primatives::vertex::getBindingDescription();
	params.vertexInputDescriptions.inputAttributeDescriptions = primatives::vertex::getAttributeDescriptions();

	shader::shader *vertex = v.createShader(params);
	if(vertex == NULL) return 0;
	
	shader::shader *fragment = v.createShader(shader::parameters(std::string("assets/shaders/compiled/shader.frag.spv"), shader::TYPE::fragment));
	if(fragment == NULL) return 0;

	std::vector<shader::shader*> shaders;

	shaders.push_back(vertex);
	shaders.push_back(fragment);

	std::vector<mesh*> meshes;

	primatives::mesh pmesh;
	pmesh.vertices = {{{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}},
    				  {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    				  {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
					  {{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}},
					  {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
					  {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},

					  {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}},
					  {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
					  {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}},
					  {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
					  {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
					  {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}},

					  {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}},
					  {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
					  {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
				  	  {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}},
					  {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
					  {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
					  					  					  					  
					  {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
					  {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
					  {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}},
					  {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
					  {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
					  {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}},

					  {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}},
    				  {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    				  {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
					  {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}},
					  {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
					  {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},

					  {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
					  {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
					  {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}},
    				  {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
					  {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
					  {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}}
    				  
					  };
	
	mesh *cube = v.createMesh(pmesh);
	if(cube == NULL) return 0;

	meshes.push_back(cube);

	std::vector<buffer*> buffers;

	class transformations
	{
	public:
		primatives::matrices::matrix4x4 world;
		primatives::matrices::matrix4x4 object;
	};

	transformations t;
	t.world = primatives::matrices::translation({0.0f,0.0f,-2.0f});
	t.object = primatives::matrices::rotation::z(0.0f);

	vulkan::buffer *buffer = v.createBuffer(&t, sizeof(transformations));
	buffers.push_back(buffer);

	float fov = 90.0f;
	float near = 0.1, far = 100.0;
	float ar = (600.0f / 800.0f);

	::vulkan::constants constants;
	constants.m = primatives::matrices::projection(fov, ar, near, far);

	pipeline *pipeline = v.createPipeline(shaders, meshes, buffers, &constants);
	if(pipeline == NULL) return 0;

	float angle = 0.0f;

	while(true)
	{
		pipeline->render();

		angle += 0.0003f;
		t.object.copy(primatives::matrices::rotation::x(angle) * primatives::matrices::rotation::z(angle));
		buffer->update();
	}

	return 0;
}

int basicMeshStorageBuffer()
{
	const int width = 800, height = 800;

	vulkan::window w(width, height);
	vulkan::vulkan v(&w);
	
	shader::parameters params(std::string("assets/shaders/compiled/storage.frag.spv"), shader::TYPE::vertex);
	params.vertexInputDescriptions.inputBindingDescription = primatives::vertex::getBindingDescription();
	params.vertexInputDescriptions.inputAttributeDescriptions = primatives::vertex::getAttributeDescriptions();

	shader::shader *vertex = v.createShader(params);
	if(vertex == NULL) return 0;
	
	shader::shader *fragment = v.createShader(shader::parameters(std::string("assets/shaders/compiled/shader.frag.spv"), shader::TYPE::fragment));
	if(fragment == NULL) return 0;

	std::vector<shader::shader*> shaders;

	shaders.push_back(vertex);
	shaders.push_back(fragment);

	std::vector<mesh*> meshes;

	primatives::mesh pmesh;
	pmesh.vertices = {{{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}},
    				  {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    				  {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
					  {{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}},
					  {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
					  {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},

					  {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}},
					  {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
					  {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}},
					  {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
					  {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
					  {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}}
					  };
	
	mesh *triangle = v.createMesh(pmesh);
	if(triangle == NULL) return 0;

	meshes.push_back(triangle);

	std::vector<buffer*> buffers;

	class transformations
	{
	public:
		primatives::matrices::matrix4x4 world;
		primatives::matrices::matrix4x4 object;
	};

	transformations t;
	t.world = primatives::matrices::translation({0.0f,0.0f,-2.0f});
	t.object = primatives::matrices::rotation::z(0.0f);

	vulkan::buffer *buffer = v.createBuffer(&t, sizeof(transformations));
	buffers.push_back(buffer);

	std::vector<primatives::matrices::matrix4x4> matrices;
	matrices.push_back(primatives::matrices::translation({-0.2f,0.0f,0.0f}));
	matrices.push_back(primatives::matrices::translation({0.2f,0.0f,0.0f}));
	vulkan::buffer *storage = v.createBuffer(matrices.data(), matrices.size() * sizeof(primatives::matrices::matrix4x4), buffer::TYPE::storage);
	buffers.push_back(storage);

	float fov = 90.0f;
	float near = 0.1, far = 100.0;
	float ar = (600.0f / 800.0f);

	::vulkan::constants constants;
	constants.m = primatives::matrices::projection(fov, ar, near, far);

	pipeline *pipeline = v.createPipeline(shaders, meshes, buffers, &constants);
	if(pipeline == NULL) return 0;

	float angle = 0.0f;
	int instanceCount = 2;
	int counter = 0;
	while(true)
	{		
		pipeline->render();

		matrices[0] = primatives::matrices::translation({-0.2f - angle,0.0f,0.0f});
		matrices[1] = primatives::matrices::translation({0.2f + angle,0.0f,0.0f});
		storage->update();
		angle += 0.00001f;

		if(++counter > 10000)
		{
			if(instanceCount == 2) instanceCount = 1;
			else instanceCount = 2;

			pipeline->update(0, instanceCount, 0);
			counter = 0;
		}
	}

	return 0;
}
*/
/*
int basicMeshMultipleObjectsWithStorageBuffer()
{
	const int width = 800, height = 800;

	vulkan::window w(width, height);
	vulkan::vulkan v(&w);
	
	shader::parameters params(std::string("assets/shaders/compiled/multi.vert.spv"), shader::TYPE::vertex);
	params.vertexInputDescriptions.inputBindingDescription = primatives::vertices::vertex::getBindingDescription();
	params.vertexInputDescriptions.inputAttributeDescriptions = primatives::vertices::vertex::getAttributeDescriptions();

	shader::shader *vertex = v.createShader(params);
	if(vertex == NULL) return 0;
	
	shader::shader *fragment = v.createShader(shader::parameters(std::string("assets/shaders/compiled/shader.frag.spv"), shader::TYPE::fragment));
	if(fragment == NULL) return 0;

	std::vector<shader::shader*> shaders;

	shaders.push_back(vertex);
	shaders.push_back(fragment);

	std::vector<mesh*> meshes;

	primatives::mesh _triangle;
	_triangle.vertices = {{{0.0f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}},
    				     {{0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
    				     {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
					     };
	
	primatives::mesh _square;
	_square.vertices = {{{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}},
    				  {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    				  {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
					  {{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}},
					  {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
					  {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}}
					  };

	mesh *triangle = v.createMesh(_triangle);
	if(triangle == NULL) return 0;

	mesh *square = v.createMesh(_square);
	if(square == NULL) return 0;

	meshes.push_back(triangle);
	meshes.push_back(square);

	std::vector<buffer*> buffers;

	class transformations
	{
	public:
		primatives::matrices::matrix4x4 world;
		primatives::matrices::matrix4x4 object;
	};

	transformations t;
	t.world = primatives::matrices::translation({0.8f,-0.8f,-3.8f});
	t.object.identity();

	vulkan::buffer *buffer = v.createBuffer(&t, sizeof(transformations));
	buffers.push_back(buffer);

	class object
	{
	public:
		primatives::matrices::matrix4x4 position;
		primatives::matrices::matrix4x4 matrix;
	};

	std::vector<object> matrices, b_matrices;

	int columns = 2, rows = 2;
	float x_increment = 1.5f, y_increment = 1.5f;
	float start_x = -((columns / 2) * x_increment);
	float start_y = -((rows / 2) * y_increment);

	for(int y = 0; y < rows; ++y)
	{
		float y_pos = start_y + (y_increment * ((float)y));
		for(int x = 0; x < columns; ++x)
		{			
			float x_pos = start_x + (x_increment * ((float)x));
			object temp;	

			temp.position = primatives::matrices::translation({x_pos,y_pos,0.0f});
			temp.matrix.identity();
			matrices.push_back(temp);

			temp.position = primatives::matrices::translation({x_pos,y_pos + ((float)rows) + y_increment,0.0f});
			b_matrices.push_back(temp);
		}
	}

	matrices.insert(matrices.end(), b_matrices.begin(), b_matrices.end());
	
	vulkan::buffer *storage = v.createBuffer(matrices.data(), matrices.size() * sizeof(object), buffer::TYPE::storage);
	buffers.push_back(storage);

	float fov = 90.0f;
	float near = 0.1, far = 100.0;
	float ar = (height / width);

	::vulkan::constants constants;
	constants.m = primatives::matrices::projection(fov, ar, near, far);

	::vulkan::depth *depth = v.createDepthBuffer();

	::vulkan::interfaces::frames *frames = v.createFrames<vulkan::frames::framesClearDepthBuffer>(depth);

	pipeline *pipeline = v.createPipeline(frames, shaders, meshes, buffers, &constants);
	if(pipeline == NULL) return 0;

	pipeline->update(0, 4, 0);
	pipeline->update(1, 4, 4);

	float angle = 0.0f;

	w.start();

	while(!w.terminated())
	{		
		pipeline->render();

		for(int i = 0; i < matrices.size() / 2; ++i)
		{
			matrices[i].matrix = primatives::matrices::rotation::z(-angle);		
		}

		for(int i = matrices.size() / 2; i < matrices.size(); ++i)
		{
			matrices[i].matrix = primatives::matrices::rotation::y(angle);		
		}

		storage->update();
		angle += 0.0005f;
	}

	return 0;
}

int basicMeshMultipleObjectsWithStorageBufferAndDynamicAddRemove()
{
	const int width = 800, height = 800;

	vulkan::window w(width, height);
	vulkan::vulkan v(&w);
	
	shader::parameters params(std::string("assets/shaders/compiled/multi.vert.spv"), shader::TYPE::vertex);
	params.vertexInputDescriptions.inputBindingDescription = primatives::vertices::vertex::getBindingDescription();
	params.vertexInputDescriptions.inputAttributeDescriptions = primatives::vertices::vertex::getAttributeDescriptions();

	shader::shader *vertex = v.createShader(params);
	if(vertex == NULL) return 0;
	
	shader::shader *fragment = v.createShader(shader::parameters(std::string("assets/shaders/compiled/shader.frag.spv"), shader::TYPE::fragment));
	if(fragment == NULL) return 0;

	std::vector<shader::shader*> shaders;

	shaders.push_back(vertex);
	shaders.push_back(fragment);

	std::vector<mesh*> meshes;

	primatives::mesh _square;
	if(!_square.load("assets/meshes/cube.obj")) return 0;

	mesh *square = v.createMesh(_square);
	if(square == NULL) return 0;

	meshes.push_back(square);

	std::vector<buffer*> buffers;

	class transformations
	{
	public:
		primatives::matrices::matrix4x4 world;
		primatives::matrices::matrix4x4 object;
	};

	transformations t;
	t.world = primatives::matrices::translation({0.8f,-0.8f,-3.8f});
	t.object.identity();

	vulkan::buffer *buffer = v.createBuffer(&t, sizeof(transformations));
	buffers.push_back(buffer);

	class object
	{
	public:
		primatives::matrices::matrix4x4 position;
		primatives::matrices::matrix4x4 matrix;
	};

	std::vector<object> matrices;

	int columns = 2, rows = 2;
	float x_increment = 1.5f, y_increment = 1.5f;
	float start_x = -((columns / 2) * x_increment);
	float start_y = -((rows / 2) * y_increment);

	for(int y = 0; y < rows; ++y)
	{
		float y_pos = start_y + (y_increment * ((float)y));
		for(int x = 0; x < columns; ++x)
		{			
			float x_pos = start_x + (x_increment * ((float)x));
			object temp;	

			temp.position = primatives::matrices::translation({x_pos,y_pos,0.0f});
			temp.matrix.identity();
			matrices.push_back(temp);
		}
	}
	
	vulkan::buffer *storage = v.createBuffer(matrices.data(), matrices.size() * sizeof(object), buffer::TYPE::storage);
	buffers.push_back(storage);

	float fov = 90.0f;
	float near = 0.1, far = 100.0;
	float ar = (height / width);

	::vulkan::constants constants;
	constants.m = primatives::matrices::projection(fov, ar, near, far);

	::vulkan::depth *depth = v.createDepthBuffer();

	::vulkan::interfaces::frames *frames = v.createFrames<vulkan::frames::framesClearDepthBuffer>(depth);

	pipeline *pipeline = v.createPipeline(frames, shaders, meshes, buffers, &constants);
	if(pipeline == NULL) return 0;

	pipeline->update(0, 4, 0);

	float angle = 0.0f;

	w.start();

	int instances = 4;

	while(!w.terminated())
	{		
		pipeline->render();

		for(int i = 0; i < matrices.size(); ++i)
		{
			matrices[i].matrix = primatives::matrices::rotation::y(angle);		
		}

		storage->update();
		angle += 0.0005f;

		char value;
		if(w.keypressed(value))
		{			
			std::cout << "value [" << value << "]\r\n";
			if((value == '-')&&(instances > 0)) --instances;
			else if((value == '+')&&(instances < 4)) ++instances;

			pipeline->update(0, instances, 0);
		}
	}

	return 0;
}

int basicMeshMultipleObjectsWithStorageBufferAndDynamicAddRemove(int cubes)
{
	std::mt19937_64 generator(std::random_device{}());
	const int width = 800, height = 800;

	vulkan::window w(width, height);
	vulkan::vulkan v(&w);
	
	shader::parameters params1(std::string("assets/shaders/compiled/dynamic.vert.spv"), shader::TYPE::vertex);
	params1.vertexInputDescriptions.inputBindingDescription = primatives::vertices::vertex::getBindingDescription();
	params1.vertexInputDescriptions.inputAttributeDescriptions = primatives::vertices::vertex::getAttributeDescriptions();

	shader::shader *vertex1 = v.createShader(params1);
	if(vertex1 == NULL) return 0;
	
	//shader::parameters params2(std::string("assets/shaders/compiled/wobble.vert.spv"), shader::TYPE::vertex);
	//params2.vertexInputDescriptions.inputBindingDescription = primatives::vertex::getBindingDescription();
	//params2.vertexInputDescriptions.inputAttributeDescriptions = primatives::vertex::getAttributeDescriptions();

	//shader::shader *vertex2 = v.createShader(params2);
	//if(vertex2 == NULL) return 0;
	

	shader::shader *fragment = v.createShader(shader::parameters(std::string("assets/shaders/compiled/shader.frag.spv"), shader::TYPE::fragment));
	if(fragment == NULL) return 0;

	std::vector<shader::shader*> shaders;

	shaders.push_back(vertex1);
	shaders.push_back(fragment);

	std::vector<mesh*> meshes;

	primatives::mesh _square;
	if(!_square.load("assets/meshes/cube.obj")) return 0;

	mesh *square = v.createMesh(_square);
	if(square == NULL) return 0;

	meshes.push_back(square);

	std::vector<buffer*> buffers;

	class transformations
	{
	public:
		primatives::matrices::matrix4x4 world;
		primatives::matrices::matrix4x4 object;
	};

	transformations t;
	t.world = primatives::matrices::translation({0.8f,-0.8f,-3.8f});
	t.object.identity();

	vulkan::buffer *buffer = v.createBuffer(&t, sizeof(transformations));
	buffers.push_back(buffer);

	class object
	{
	public:
		primatives::matrices::matrix4x4 position;
		primatives::matrices::matrix4x4 matrix;
		primatives::vector3 colour;
		float padding;
	};

	class cube
	{
	public:
		float x,y,z;
		bool visible;

		float red,green,blue;

	public:
		cube() { x = y = z = 0.0f; visible = false; }

		void update()
		{
			y += 0.001f;
			if(y > 6.0f) 
			{
				std::cout << "visible false\r\n";
				visible = false;
			}
		}
	};

	std::vector<cube> data;
	std::vector<object> matrices;

	for(int i = 0; i < cubes; ++i)
	{
		object temp;

		temp.position = primatives::matrices::translation({0.0f,0.0f,0.0f});
		temp.matrix.identity();

		matrices.push_back(temp);
		data.push_back(cube());
	}
	
	vulkan::buffer *storage = v.createBuffer(matrices.data(), matrices.size() * sizeof(object), buffer::TYPE::storage);
	buffers.push_back(storage);

	float fov = 90.0f;
	float near = 0.1, far = 100.0;
	float ar = (height / width);

	::vulkan::constants constants;
	constants.m = primatives::matrices::projection(fov, ar, near, far);

	::vulkan::depth *depth = v.createDepthBuffer();

	::vulkan::interfaces::frames *frames = v.createFrames<vulkan::frames::framesClearDepthBuffer>(depth);

	pipeline *pipeline = v.createPipeline(frames, shaders, meshes, buffers, &constants);
	if(pipeline == NULL) return 0;

	pipeline->update(0, 0, 0);

	const int COLUMNS = 6;
	std::vector<int> columns(COLUMNS);
	for(int i = 0; i < COLUMNS; ++i) columns[i] = 0;

	w.start();	

	while(!w.terminated())
	{		
		pipeline->render();

		int count = 0;
		for(std::vector<cube>::iterator it = data.begin(); it != data.end(); ++it)		
		{
			if(it->visible) 			
			{
				matrices[count].matrix = primatives::matrices::translation({it->x,it->y,it->z});
				matrices[count].colour = primatives::vector3(it->red,it->green,it->blue);
				it->update();
				++count;
			}
		}

		storage->update();
		//angle += 0.0005f;

		char value;
		if(w.keypressed(value))
		{			
			std::cout << "value [" << value << "]\r\n";

			if((value == ' ')&&(count < cubes)) 
			{				
				for(std::vector<cube>::iterator it = data.begin(); it != data.end(); ++it)				
				{
					if(!it->visible) 
					{						
						int t1 = (std::uniform_int_distribution<int>{0, COLUMNS - 1})(generator);
						if(columns[t1] == 0)
						{
							columns[t1] = 2000;

							it->visible = true;
							it->x = (t1 * 1.5f) - 4.0f;
							it->y = -3.5f;
							it->z = 0.0f;

							it->red = 0.0f; it->green = 0.0f; it->blue = 0.0f;
							int t2 = (std::uniform_int_distribution<int>{0, 2})(generator);
							if(t2 == 0) it->red = 1.0f;
							else if(t2 == 1) it->green = 1.0f;
							else if(t2 == 2) it->blue = 1.0f;

							std::cout << "add " << it->x << " " << count << "\r\n";
						}

						break;
					}
				}				
			}			
		}

		pipeline->update(0, count, 0);

		for(int i = 0; i < COLUMNS; ++i) 
		{
			if(columns[i] > 0) columns[i] -= 1;
		}
	}

	return 0;
}
*/
// display font
// texture mapping
// display frames per second
// work out consistent speed of objects from frames per second

int main(int argc, char *argv[])
{
	//basicVertexAndFragmentShaders();
	//basicMeshShaders();
	//basicLoadObjMeshShaders();
	//basicMeshProjection();
	//basicMeshStorageBuffer();
	//basicMeshMultipleObjectsWithStorageBuffer();	
	//basicMeshMultipleObjectsWithStorageBufferAndDynamicAddRemove(50);

	engine::engine e(engine::settings(800,800));
	e.run();
	
	return 0;
}