#include "tracer/frame.h"
#include <sstream>
#include <functional>

bool tracer::frame::get(trace &destination, int index)
{
    if((index < 0)||(index >= data.size())) return false;
    destination = data[index];
    return true;
}

void tracer::frame::deserialise(std::string source)
{
    tracer::trace temp;
    temp.deserialise(source);
    data.push_back(temp);
}
