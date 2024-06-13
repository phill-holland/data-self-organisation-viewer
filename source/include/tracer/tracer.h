#include "tracer/frame.h"
#include <vector>
#include <string>

#ifndef _TRACER_TRACER
#define _TRACER_TRACER

namespace tracer
{
    class tracer
    {
        std::vector<frame> data;

    public:
        tracer() { clear(); }

        void clear()
        {
            data.clear();
        }

        bool isempty() { return data.size() == 0; }
        std::size_t size() { return data.size(); }
        
        bool get(frame &destination, int index);

        bool load(std::string filename);
    };
};

#endif