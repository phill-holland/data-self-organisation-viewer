#include "graphics/targa.h"
#include <iostream>
#include <fstream>
#include <memory.h>

bool graphics::targa::targa::load(std::string filename)
{
	return load(filename.c_str());
}

bool graphics::targa::targa::save(std::string filename)
{
	return save(filename.c_str());
}

bool graphics::targa::targa::load(const char *filename)
{
	bool result = false;

	header head;

	std::ifstream handle;
	handle.open(filename, std::ios::in | std::ios::binary);

	handle.read(reinterpret_cast<char*>(&head), sizeof(header));

	if ((head.image_type == TGA_RGB) && (head.bpp == 32))
	{
		if ((head.width == (unsigned short)source->width) && (head.height == (unsigned short)source->height))
		{
			long bytesToRead = (long)source->width * (long)source->height * 4L;
			if (head.offset > 0) handle.seekg((int)head.offset, handle.cur);

			const unsigned long LENGTH = 1024UL;
			unsigned long length = source->width * source->height;
			unsigned long index = 0UL;

			colour buffer[LENGTH];
			while(length > 0UL)
			{
				int toRead = LENGTH;
				if(length < LENGTH) toRead = length;

				handle.read(reinterpret_cast<char*>(buffer), toRead * 4);
				for(int i = 0; i < toRead; ++i)
				{
					source->data[index].blue = buffer[i].red;
					source->data[index].green = buffer[i].green;
					source->data[index].red = buffer[i].blue;
					source->data[index].alpha = buffer[i].alpha;
					++index;
				}

				length -= toRead;
			};			

			result = true;
		}
	}

	handle.close();

	return result;
}

bool graphics::targa::targa::save(const char *filename)
{
	header head;

	memset(&head, 0, sizeof(header));
	head.offset = 0;
	head.bpp = 32;
	head.image_type = TGA_RGB;
	head.height = (unsigned short)source->height;
	head.width = (unsigned short)source->width;
	head.colour_type = 0;
	head.x_origin = 0;
	head.y_origin = 0;

	std::ofstream handle;
	handle.open(filename, std::ios::out | std::ios::trunc | std::ios::binary);
	
	long bytesToWrite = (long)source->width * (long)source->height * 4L;

	handle.write(reinterpret_cast<char*>(&head), sizeof(header));

	const unsigned long LENGTH = 1024UL;
	unsigned long length = source->width * source->height;
	unsigned long index = 0UL;

	colour buffer[LENGTH];
	
	for(unsigned long i = 0UL; i < length; ++i)
	{
		buffer[index].red = source->data[i].blue;
		buffer[index].green = source->data[i].green;
		buffer[index].blue = source->data[i].red;
		buffer[index].alpha = source->data[i].alpha;
		++index;
		if(index >= LENGTH)
		{
			handle.write(reinterpret_cast<char*>(buffer), index * 4UL);
			index = 0UL;
		}
	}

	if(index >= LENGTH) handle.write(reinterpret_cast<char*>(buffer), index * 4UL);

	handle.close();

	return true;
}