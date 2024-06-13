#include <string>
#include "graphics/texture.h"
#include "graphics/colour.h"

#ifndef _GRAPHICS_TARGA
#define _GRAPHICS_TARGA

namespace graphics
{
	namespace targa
	{
		class header
		{
		public:
			unsigned char offset;
			unsigned char colour_type;
			unsigned char image_type;
			unsigned char colour_map[5];
			unsigned short x_origin, y_origin;
			unsigned short width, height;
			unsigned char bpp;
			unsigned char description;
		};

		class targa
		{
		protected:
			texture *source;

		protected:		
			static const unsigned long TGA_RGB = 2UL;
			
		public:
			targa(texture *src) { source = src; }
			
			bool load(std::string filename);
			bool save(std::string filename);

			bool load(const char *filename);
			bool save(const char *filename);
		};
	};
};

#endif