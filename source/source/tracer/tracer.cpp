#include "tracer/tracer.h"
#include "tracer/frame.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>

bool tracer::tracer::get(frame &destination, int index)
{
    if((index < 0)||(index >= data.size())) return false;
    destination = data[index];
    return true;
}

bool tracer::tracer::load(std::string filename)
{
    std::ifstream source(filename);
    if(!source.is_open()) return false;

    ::tracer::frame temp;

    for(std::string value; std::getline(source,value); )
    {
        if(value.size() != 0)
        {
            temp.deserialise(value);
        }
        else if(!temp.isempty())
        {
            data.push_back(temp);
            temp.clear();
        }
    }

    source.close();

    return true;
}
