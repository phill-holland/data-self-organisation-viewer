#include "graphics/colour.h"

#ifndef _GRAPHICS_TEXTURE
#define _GRAPHICS_TEXTURE

namespace graphics
{
	class texture
	{				
		bool init;

	public:
		unsigned long width,height;
		unsigned long length;

		colour *data;

	public:
		texture(unsigned long w, unsigned long h) { makeNull(); reset(w, h); }
		texture() { makeNull(); width = 0UL; height = 0UL; length = 0UL; init = false; }

		bool initalised() { return init; }

		void reset(unsigned long w, unsigned long h);

		void set(unsigned long x, unsigned long y, colour c);

		colour get(unsigned long x, unsigned long y);
		colour get(float x, float y);
		
		void clear(colour c);
			
		void line(unsigned long minX, unsigned long minY, unsigned long maxX, unsigned long maxY, colour c);
		void circle(unsigned long x, unsigned long y, unsigned long radius, colour c);
		void ellipse(unsigned long x, unsigned long y, unsigned long w, unsigned long h, colour c, bool fill);

	public:
		bool into(void *data, int length);

	public:
		void copy(texture &src);

	protected:
		void makeNull();
		void cleanup();
	};	
};

#endif