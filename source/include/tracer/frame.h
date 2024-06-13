#include "tracer/trace.h"
#include <vector>
#include <string>

#ifndef _TRACER_FRAME
#define _TRACER_FRAME

namespace tracer
{
    class frame
    {
        std::vector<trace> data;

    public:
        frame() { clear(); }

        void clear()
        {
            data.clear();
        }

        bool isempty() { return data.size() == 0; }
        std::size_t size() { return data.size(); }

        bool get(trace &destination, int index);

        void deserialise(std::string source);
    };
};

#endif
// std:vector<trace> data;

// amd tracer

// std::vector<frame> data;