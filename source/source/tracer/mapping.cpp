#include "tracer/mapping.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>

std::string tracer::mapping::get(int index)
{
    std::string result;

    if(mappings.find(index) != mappings.end())
        result = mappings[index];

    return result;
}

std::string tracer::mapping::get(point data)
{
    std::string result;

    int values[] = { data.x, data.y, data.z };
    
    for(int i = 0; i < 3; ++i)
    {
        if(values[i] != -1)
        {
            if(result.size() != 0) result += ",";
            result += get(values[i]);
        }
    }

    return result;
}

bool tracer::mapping::load(std::string filename)
{
    std::ifstream source(filename);
    if(!source.is_open()) return false;

    for(std::string value; std::getline(source,value); )
    {
        if(value.size() != 0)
        {
            std::stringstream input_stream(value);
            
            int identity = 0;
            int index = 0;
            
            for(std::string input; std::getline(input_stream, input, ' '); )
            {
                if(index == 0)
                {
                    identity = std::atoi(input.c_str());
                }
                else if(index == 1)
                {
                    mappings[identity] = input;
                }
            }
        }
    }

    source.close();

    return true;
}