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
#include <filesystem>

using namespace vulkan;

std::vector<std::string> list_dir(std::string directory, std::string search)
{
	struct path_str
	{
		std::string operator()(const std::filesystem::directory_entry &entry) const
		{
			return entry.path().filename().string();
		}
	};

	std::vector<std::string> result, intermediate;

	std::filesystem::path path(directory);

	std::filesystem::directory_iterator start(path), end;
	std::transform(start, end, std::back_inserter(intermediate), path_str());

	for(auto &filename:intermediate)
	{
		if(filename.find(search) != std::string::npos)
			result.push_back(filename);	
	}

	return result;
}

std::string selection(const std::string directory)
{
	int index = 0;
	std::vector<std::string> files = list_dir(directory, ".txt");
	for(auto &it: files)
	{
		std::cout << std::to_string(index++) << ") " << it << "\r\n";
	}

	int selected = -1;
	do
	{
		std::string value;
		std::cin >> value;

		selected = atoi(value.c_str());
		if((selected < 0)||(selected > files.size()))
			selected = -1;

	}while(selected == -1);

	return directory + "/" + files[selected];
}

int main(int argc, char *argv[])
{
	const std::string directory("assets/data/traces");
	std::string filename = selection(directory);

	tracer::tracer data;

	if(data.load(filename))
	{
		engine::engine e(engine::settings(800, 800), data);
		e.run();
	}

	return 0;
}