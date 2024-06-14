#include "point.h"
#include <string>
#include <unordered_map>

#ifndef _TRACER_MAPPING
#define _TRACER_MAPPING

namespace tracer
{
    class mapping
    {
        std::unordered_map<int,std::string> mappings;

    public:
        mapping() { clear(); }

        void clear() { mappings.clear(); }

        std::string get(int index);
        std::string get(point data);

        bool load(std::string filename);
    };
};

#endif