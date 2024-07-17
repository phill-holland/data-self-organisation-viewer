
#ifndef _GRAPHICS_COLOUR
#define _GRAPHICS_COLOUR

namespace graphics
{
	class colour
	{
	public:
		unsigned char red, green, blue, alpha;

	public:
		colour() { blue = 0; green = 0; red = 0; alpha = 255; }
		colour(unsigned char r, unsigned char g, unsigned char b) { red = r; green = g;  blue = b; alpha = 255; }
		colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { red = r; green = g; blue = b; alpha = a; }

		void fromYCrCb(int y, int cr, int cb)
		{
			int c = y - 16;
			int d = cb - 128;
			int e = cr - 128;

			red = clip((298 * c + 409 * e + 128) >> 8);
			green = clip((298 * c - 100 * d - 208 * e + 128) >> 8);
			blue = clip((298 * c + 516 * d + 128) >> 8);
		}

		void copy(colour &src) { blue = src.blue; green = src.green; red = src.red; alpha = src.alpha; }

	protected:
		unsigned char clip(int source)
		{
			return (unsigned char)(source < 0 ? 0 : (source > 255 ? 255 : source));
		}

	public:
		colour operator=(colour src) { red = src.red; green = src.green; blue = src.blue; alpha = src.alpha; return *this; }
		bool operator==(colour src) { return ((red==src.red)&&(green==src.green)&&(blue==src.blue)); }
	};
};

#endif