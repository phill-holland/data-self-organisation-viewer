#include "engine.h"
#include "graphics/vulkan/vertices/colour.h"
#include "graphics/vulkan/vertices/texture.h"
#include "graphics/vulkan/frames.h"
#include "graphics/texture.h"
#include "graphics/targa.h"

std::mt19937_64 engine::engine::generator(std::random_device{}());

void engine::engine::background(core::thread *bt)
{
	const double interval = 200.0;

	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now(); 		
	std::chrono::milliseconds time_span = std::chrono::duration_cast<std::chrono::milliseconds>(now - previous);   
	previous = now;
	
	accumlator += time_span.count();

	if(accumlator > interval)
	{
		accumlator -= interval;

		a->update(particles);		
		//b->update(particles);
		//c->update(particles);
	}

	sleep(5);
}

void engine::engine::reset(settings _settings, tracer::tracer data)
{
    init = false; cleanup();

	accumlator = 0.0; //currentFrame = 0;

	this->data = data;

    window = new vulkan::window(_settings.width, _settings.height);
    if(window == NULL) return;
	
    vulkan = new vulkan::vulkan(window);
    if(vulkan == NULL) return;
    if(!vulkan->initalised()) return;

    camera = new camera::camera(vulkan,camera::settings(_settings.width,_settings.height));
    if(camera == NULL) return;
    if(!camera->initalised()) return;

    particles = new ::engine::particles(total);
    if(particles == NULL) return;
    if(!particles->initalised()) return;

	particles->clear();
	/*
    for(int i = 0; i < total; ++i)
    {
        particles->values[i].type = 0;//i % 3;
    }
	*/	

	/*	
	graphics::texture tt(256,256);
	tt.clear(graphics::colour(127,0,0));
	tt.circle(55,55,20,graphics::colour(255,255,0));
	tt.circle(155,155,20,graphics::colour(255,255,127));

	graphics::targa::targa targa1(&tt);
	targa1.save(std::string("assets/images/test.tga"));
	*/
/*
	graphics::texture texture(256,256);
	graphics::targa::targa targa(&texture);
	if(!targa.load(std::string("assets/images/test.tga"))) return;

	this->texture = vulkan->createTexture(256, 256);
	if(this->texture == NULL) return;	
	//this->texture = NULL;
*/
	depth = vulkan->createDepthBuffer();

	clearDepthFrames = vulkan->createFrames<vulkan::frames::framesClearDepthBuffer>(depth);
	if(clearDepthFrames == NULL) return;

	reuseDepthFrames = vulkan->createFrames<vulkan::frames::framesReuseDepthBuffer>(depth);
	if(reuseDepthFrames == NULL) return;

	vulkan::vertices::colour::descriptor d1;
    a = new pipeline::pipeline(vulkan, clearDepthFrames, camera, &d1, NULL, particles, 0, pipeline::settings(std::string("assets/shaders/compiled/shader.frag.spv"),std::string("assets/shaders/compiled/dynamic.vert.spv"),std::string("assets/meshes/cube2.obj")));
    if(a == NULL) return;
    if(!a->initalised()) return;

/*
    b = new pipeline::pipeline(vulkan, reuseDepthFrames, camera, &d1, NULL, particles, 1, pipeline::settings(std::string("assets/shaders/compiled/shader.frag.spv"),std::string("assets/shaders/compiled/wobble.vert.spv"),std::string("assets/meshes/cube2.obj")));
    if(b == NULL) return;
    if(!b->initalised()) return;

	vulkan::vertices::texture::descriptor d2;
	c = new pipeline::pipeline(vulkan, reuseDepthFrames, camera, &d2, this->texture, particles, 2, pipeline::settings(std::string("assets/shaders/compiled/texture.frag.spv"),std::string("assets/shaders/compiled/texture.vert.spv"),std::string("assets/meshes/cube.obj")));
    if(c == NULL) return;
    if(!c->initalised()) return;
*/
/*
	if(this->texture != NULL)
	{
		this->texture->copy(&texture);
		this->texture->transfer(c->pipe);
	}
*/
    init = true;
}

void engine::engine::run()
{    
	const float grid_w = 6.0f, grid_h = 6.0f, grid_d = 6.0f;
	const float offset_x = -(grid_w / 2.0f);
	const float offset_y = -(grid_h / 2.0f);
	const float offset_z = -grid_d;
	const float transisition_inc = 0.001f;
	const float max_words = 36.0f;
	const float camera_inc = 0.09f;

	float camera_x = 0.8f, camera_y = -0.8f, camera_z = -3.8f;

	tracer::frame current_frame, previous_frame;
	bool transistion = false;
	float transisition_offset = 0.0f;

	int trace = 0, previous_epoch = 0;
	
	window->start();	
	this->start();

	int frame = 0;
	std::chrono::high_resolution_clock::time_point previous = std::chrono::high_resolution_clock::now();

	while(!window->terminated())
	{	
		++frame;
		const double max = 500;
		if(frame > ((int)max))
		{
			std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now(); 		
			std::chrono::milliseconds time_span = std::chrono::duration_cast<std::chrono::milliseconds>(now - previous);   
			previous = now;

			double ms = (double)time_span.count();

			double fps = (1000.0 / (ms / max));
//std::cout << "fps " << fps << " ms " << ms << "\r\n";
			frame = 0;
		}

		std::vector<vulkan::pipeline*> pipelines = { a->pipe };//, b->pipe, c->pipe };
		vulkan->renderPipelines(pipelines);

		char value;
		if(window->keypressed(value))
		{
			// ***
			if(!transistion)
			{
				if(value == ' ')
				{
					tracer::frame frame;
					if(data.get(frame, trace++))
					{
						previous_frame = current_frame;
						current_frame = frame;

						transistion = true;
						transisition_offset = 0.0f;

						for(int i = 0; i < current_frame.size(); ++i)
						{
							tracer::trace current_trace;
							if(current_frame.get(current_trace, i))
							{
								if(current_trace.epoch != previous_epoch) previous_frame.clear();
								previous_epoch = current_trace.epoch;

								std::cout << "add h=" << (current_trace.stationary ? "1" : "0") << " ";
								std::cout << "e=" << current_trace.epoch << " ";
								std::cout << "pos(" << current_trace.position.x << "," << current_trace.position.y << "," << current_trace.position.z << ") ";
								std::cout << "data(" << current_trace.data.x << "," << current_trace.data.y << "," << current_trace.data.z << ") Col=" + current_trace.collision + " Lnk=" + current_trace.links + "\r\n";
							}
						}				
					}

					std::cout << "\r\n";
				}
				else if (value == 'a')
				{
					camera_x -= camera_inc;
					camera->position(camera_x,camera_y,camera_z);
					camera->buffer->update();					
				}
				else if (value == 'd')
				{
					camera_x += camera_inc;
					camera->position(camera_x,camera_y,camera_z);
					camera->buffer->update();					
				}
				else if (value == 'w')
				{
					camera_y -= camera_inc;
					camera->position(camera_x,camera_y,camera_z);
					camera->buffer->update();					
				}
				else if (value == 's')
				{
					camera_y += camera_inc;
					camera->position(camera_x,camera_y,camera_z);
					camera->buffer->update();					
				}
				else if (value == 'r')
				{
					camera_z -= camera_inc;
					camera->position(camera_x,camera_y,camera_z);
					camera->buffer->update();					
				}
				else if (value == 'f')
				{
					camera_z += camera_inc;
					camera->position(camera_x,camera_y,camera_z);
					camera->buffer->update();					
				}
			}
		}

		if(transistion)
		{
			particles->clear();

			for(int i = 0; i < current_frame.size(); ++i)
			{
				tracer::trace current_trace;
				if(current_frame.get(current_trace, i))
				{
					particle &p = particles->values.at(i);					
					bool stationary = false;

					tracer::trace previous_trace;
					if(previous_frame.find(previous_trace, current_trace.data))
					{
						// transistion
						tracer::point start = previous_trace.position;
						tracer::point end = current_trace.position;

						if(start != end)
						{
							float delta_x = (float)(end.x - start.x) * transisition_offset;
							float delta_y = (float)(end.y - start.y) * transisition_offset;
							float delta_z = (float)(end.z - start.z) * transisition_offset;

							p.x = offset_x + (((float)start.x) + delta_x);
							p.y = offset_y + (((float)start.y) + delta_y);
							p.z = offset_z + (((float)start.z) + delta_z);
						}
						else
						{
							p.x = offset_x + (float)end.x;
							p.y = offset_y + (float)end.y;
							p.z = offset_z + (float)end.z;
						}

						stationary = previous_trace.stationary | current_trace.stationary;
									
						p.type = 0;

						p.red = 0.0f;
						p.green = 0.0f;
						p.blue = 0.0f;

						if(stationary)
						{
							p.green = 1.0f;
							p.blue = 1.0f;
						}
						else
						{							
							p.red = ((float)current_trace.data.x) / max_words;
							if(current_trace.data.y != -1) p.green = ((float)current_trace.data.y) / max_words;
							if(current_trace.data.z != -1) p.blue = ((float)current_trace.data.z) / max_words;
						}

						p.visible = true;
					}					
				}
			} 

			transisition_offset += transisition_inc;
			if(transisition_offset >= 1.0f) transistion = false;
		}
	};
}

void engine::engine::makeNull()
{
    window = NULL;
    vulkan = NULL;
    camera = NULL;
    particles = NULL;
    a = NULL;
    //b = NULL;
	//c = NULL;
}

void engine::engine::cleanup()
{
	//if(c != NULL) delete c;
	//if(b != NULL) delete b;
    if(a != NULL) delete a;
    
    if(particles != NULL) delete particles;
    if(camera != NULL) delete camera;
    if(vulkan != NULL) delete vulkan;
    if(window != NULL) delete window;
}