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
		b->update(particles);
		c->update(particles);
	}

	sleep(5);
}

void engine::engine::reset(settings _settings)
{
    init = false; cleanup();

	accumlator = 0.0;

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

    for(int i = 0; i < total; ++i)
    {
        particles->values[i].type = i % 3;
    }

	/*	
	graphics::texture tt(256,256);
	tt.clear(graphics::colour(127,0,0));
	tt.circle(55,55,20,graphics::colour(255,255,0));
	tt.circle(155,155,20,graphics::colour(255,255,127));

	graphics::targa::targa targa1(&tt);
	targa1.save(std::string("assets/images/test.tga"));
	*/

	graphics::texture texture(256,256);
	graphics::targa::targa targa(&texture);
	if(!targa.load(std::string("assets/images/test.tga"))) return;

	this->texture = vulkan->createTexture(256, 256);
	if(this->texture == NULL) return;	
	//this->texture = NULL;

	depth = vulkan->createDepthBuffer();

	clearDepthFrames = vulkan->createFrames<vulkan::frames::framesClearDepthBuffer>(depth);
	if(clearDepthFrames == NULL) return;

	reuseDepthFrames = vulkan->createFrames<vulkan::frames::framesReuseDepthBuffer>(depth);
	if(reuseDepthFrames == NULL) return;

	vulkan::vertices::colour::descriptor d1;
    a = new pipeline::pipeline(vulkan, clearDepthFrames, camera, &d1, NULL, particles, 0, pipeline::settings(std::string("assets/shaders/compiled/shader.frag.spv"),std::string("assets/shaders/compiled/dynamic.vert.spv"),std::string("assets/meshes/cube2.obj")));
    if(a == NULL) return;
    if(!a->initalised()) return;

    b = new pipeline::pipeline(vulkan, reuseDepthFrames, camera, &d1, NULL, particles, 1, pipeline::settings(std::string("assets/shaders/compiled/shader.frag.spv"),std::string("assets/shaders/compiled/wobble.vert.spv"),std::string("assets/meshes/cube2.obj")));
    if(b == NULL) return;
    if(!b->initalised()) return;

	vulkan::vertices::texture::descriptor d2;
	c = new pipeline::pipeline(vulkan, reuseDepthFrames, camera, &d2, this->texture, particles, 2, pipeline::settings(std::string("assets/shaders/compiled/texture.frag.spv"),std::string("assets/shaders/compiled/texture.vert.spv"),std::string("assets/meshes/cube.obj")));
    if(c == NULL) return;
    if(!c->initalised()) return;

	if(this->texture != NULL)
	{
		this->texture->copy(&texture);
		this->texture->transfer(c->pipe);
	}

    init = true;
}

void engine::engine::run()
{
    const int COLUMNS = 6;
	std::vector<int> columns(COLUMNS);
	for(int i = 0; i < COLUMNS; ++i) columns[i] = 0;

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
			std::cout << "fps " << fps << " ms " << ms << "\r\n";
			frame = 0;
		}

		std::vector<vulkan::pipeline*> pipelines = { a->pipe, b->pipe, c->pipe };
		vulkan->renderPipelines(pipelines);

		char value;
		if(window->keypressed(value))
		{			
			if(value == ' ')
			{				
				for(std::vector<particle>::iterator it = particles->values.begin(); it != particles->values.end(); ++it)				
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

                            //int t3 = (std::uniform_int_distribution<int>{0, 1})(generator);
							int t3 = (std::uniform_int_distribution<int>{0, 2})(generator);
                            it->type = t3;
							//std::cout << "add " << it->x << " " << count << "\r\n";
						}

						break;
					}
				}				
			}			
		}

		for(int i = 0; i < COLUMNS; ++i) 
		{
			if(columns[i] > 0) columns[i] -= 1;
		}
	}
}

void engine::engine::makeNull()
{
    window = NULL;
    vulkan = NULL;
    camera = NULL;
    particles = NULL;
    a = NULL;
    b = NULL;
	c = NULL;
}

void engine::engine::cleanup()
{
	if(c != NULL) delete c;
	if(b != NULL) delete b;
    if(a != NULL) delete a;
    
    if(particles != NULL) delete particles;
    if(camera != NULL) delete camera;
    if(vulkan != NULL) delete vulkan;
    if(window != NULL) delete window;
}