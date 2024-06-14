#include "tracer/point.h"
#include <string>

#ifndef _TRACER_TRACE
#define _TRACER_TRACE

namespace tracer
{
    class trace
    {
    public:
        point position;
        point data; 

        int epoch;
        bool stationary;

        std::string collision;
        std::string links;

    public:
        trace() { clear(); }

        void clear()
        {
            position.clear();
            data.clear();

            epoch = 0;
            stationary = false;
            collision.clear();
            links.clear();
        }

        void deserialise(std::string source);
    };
};

#endif