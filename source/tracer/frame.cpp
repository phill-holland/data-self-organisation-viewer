#include "tracer/frame.h"
#include <sstream>
#include <functional>

bool tracer::frame::get(trace &destination, int index)
{
    if((index < 0)||(index >= data.size())) return false;
    destination = data[index];
    return true;
}

bool tracer::frame::find(trace &destination, point value)
{
    for(auto &it:data)
    {
        if(it.data == value)
        {
            destination = it;
            return true;
        }
    }

    return false;
}
        
void tracer::frame::deserialise(std::string source)
{
    tracer::trace temp;
    temp.deserialise(source);
    data.push_back(temp);
}
