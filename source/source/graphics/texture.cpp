#include "graphics/texture.h"
#include <math.h>
#include <string.h>

void graphics::texture::reset(unsigned long w, unsigned long h)
{
	init = false; cleanup();
	width = w; height = h;
	length = width * height;

	data = new colour[length];
	if (data == NULL) return;

	init = true;
}

void graphics::texture::set(unsigned long x, unsigned long y, colour c)
{
	if ((x >= 0UL) && (x<width))
		if ((y >= 0UL) && (y<height))
			data[(y * width) + x] = c;
}

graphics::colour graphics::texture::get(unsigned long x, unsigned long y)
{
	if ((x >= 0UL) && (x<width))
		if ((y >= 0UL) && (y<height))
			return data[(y * width) + x];

	return colour(0, 0, 0);
}

graphics::colour graphics::texture::get(float x, float y)
{
	unsigned long xOffset = (unsigned long)(x * (float)width);
	unsigned long yOffset = (unsigned long)(y * (float)height);

	return data[(yOffset * width) + xOffset];
}

void graphics::texture::clear(colour c)
{
	for (unsigned long i = 0UL; i < length; i++) data[i] = c;
}

void graphics::texture::line(unsigned long minX, unsigned long minY, unsigned long maxX, unsigned long maxY, colour c)
{
	long lineWidth;
	long lineHeight;

	if (minX == maxX)
	{
		long smallestY = minY, largestY = maxY;
		if (minY > maxY) { smallestY = maxY; largestY = minY; }

		for (long i = smallestY; i < largestY; i++) set(minX, i, c);

		return;
	}
	if (minY == maxY)
	{
		long smallestX = minX, largestX = maxX;
		if (minX > maxX) { smallestX = maxX; largestX = minX; }

		for (long i = smallestX; i < largestX; i++) set(i, minY, c);

		return;
	}

	long smallestX = minX, largestX = maxX;
	long smallestY = minY, largestY = maxY;

	if (minX > maxX) { smallestX = maxX; largestX = minX; }

	lineWidth = largestX - smallestX;
	lineHeight = largestY - smallestY;

	if (lineWidth > abs(lineHeight))
	{
		float gradient = (float)lineHeight / (float)lineWidth;
		float Y;

		float startY = (float)smallestY;
		if (smallestX != minX) { startY = (float)maxY; gradient *= -1.0f; }

		for (long X = 0L; X < lineWidth; X++)
		{
			Y = startY + (((float)X) * gradient);
			set(X + smallestX, (long)Y, c);
		}
	}
	else
	{
		float gradient = (float)lineWidth / (float)lineHeight;
		float X;

		float startX = (float)smallestX;
		if (smallestX != minX) { startX = (float)largestX; gradient *= -1.0f; }
		if (lineHeight < 0) { gradient *= -1.0f; }

		for (long Y = 0L; Y < abs(lineHeight); Y++)
		{
			X = startX + (((float)Y) * gradient);
			if (lineHeight < 0)
				set((long)X, smallestY - Y, c);
			else set((long)X, Y + smallestY, c);
		}
	}
}

void graphics::texture::circle(unsigned long x, unsigned long y, unsigned long radius, colour c)
{
	long x1 = 0;
	long y1 = (long)radius;
	long d = 3 - (2 * y1);

	while (x1 <= y1)
	{
		set(x + x1, y + y1, c);
		set(x - x1, y + y1, c);
		set(x + x1, y - y1, c);
		set(x - x1, y - y1, c);

		set(x + y1, y + x1, c);
		set(x + y1, y - x1, c);
		set(x - y1, y + x1, c);
		set(x - y1, y - x1, c);

		if (d < 0)
		{
			d = d + (4 * x1) + 6;
		}
		else
		{
			d = d + 4 * (x1 - y1) + 10;
			--y1;
		}
		++x1;
	}
}

void graphics::texture::ellipse(unsigned long x, unsigned long y, unsigned long w, unsigned long h, colour c, bool fill)
{
	long hh = (long)(h * h);
	long ww = (long)(w * w);
	long hhww = hh * ww;
	long x0 = (long)w;
	long dx = 0L;

	if (fill)
	{
		for (long x1 = -(long)w; x1 <= (long)w; x1++)
			set(x + x1, y, c);
	}
	else
	{
		set(x - w, y, c);
		set(x + w, y, c);
	}

	for (long y1 = 0L; y1 <= (long)h; y1++)
	{
		long x1 = x0 - (dx - 1);
		for (; x1 > 0; x1--)
			if (x1*x1*hh + y1*y1*ww <= hhww)
				break;
		dx = x0 - x1;
		x0 = x1;

		if (fill)
		{
			for (long x2 = -x0; x2 <= x0; x2++)
			{
				set(x + x2, y - y1, c);
				set(x + x2, y + y1, c);
			}
		}
		else
		{
			set(x - x0, y - y1, c);
			set(x + x0, y - y1, c);

			set(x - x0, y + y1, c);
			set(x + x0, y + y1, c);
		}
	}
}

bool graphics::texture::into(void *data, int length)
{
	if((width * height * sizeof(colour)) != length) return false;

	memcpy(data, this->data, length);

	return true;
}

void graphics::texture::copy(texture &src)
{
	if ((width == src.width) && (height == src.height))
	{
		unsigned long yoffset = 0UL;
		for (unsigned long y = 0UL; y<height; ++y)
		{
			for (unsigned long x = 0UL; x<width; ++x)
			{
				data[(yoffset + x)] = src.data[(yoffset + x)];
			}
			yoffset += width;
		}
	}
}

void graphics::texture::makeNull()
{
	data = NULL;
}

void graphics::texture::cleanup()
{
	if (data != NULL) delete[] data;
}
