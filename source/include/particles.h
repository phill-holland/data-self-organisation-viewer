#include "particle.h"
#include <vector>

#ifndef _ENGINE_PARTICLES
#define _ENGINE_PARTICLES

namespace engine
{
    class particles
	{
        bool init;

    public:
        std::vector<particle> values;

    public:
        particles(int total) { makeNull(); reset(total); }
        ~particles() { makeNull(); }

        void reset(int total);
        bool initalised() { return init; }

        size_t size() { return values.size(); }

    protected:
        void makeNull() { }
        void cleanup() { }
    };
};

#endif