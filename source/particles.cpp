#include "particles.h"

void engine::particles::reset(int total)
{
    init = false; cleanup();

	for(int i = 0; i < total; ++i)
	{
		values.push_back(particle());
	}

    init = true;
}