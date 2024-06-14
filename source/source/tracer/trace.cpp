#include "tracer/trace.h"
#include <sstream>
#include <functional>
#include <iostream>

void tracer::trace::deserialise(std::string source)
{
    std::stringstream ss_spaces(source);
    std::string str_spaces;

    while(std::getline(ss_spaces,str_spaces,' '))
    {
        std::stringstream ss_equals(str_spaces);
        std::string str_equals;

        std::string name;
        int index = 0;

        while(std::getline(ss_equals,str_equals,'='))
        {
            if(index == 0)
            {
                name = str_equals;
            }
            else if(index == 1)
            {
                if(name.compare("H") == 0)
                {
                    int temp = std::atoi(str_equals.c_str());
                    if(temp == 0) stationary = false;
                    else stationary = true;
                }
                if(name.compare("Pos") == 0)
                {
                    position.deserialise(str_equals);
                }
                else if(name.compare("Data") == 0)
                {
                    data.deserialise(str_equals);
                }
                else if(name.compare("Col") == 0)
                {
                    collision = str_equals;
                }
                else if(name.compare("Lnk") == 0)
                {
                    links = str_equals;
                }
                else if(name.compare("E") == 0)
                {
                    epoch = std::atoi(str_equals.c_str());
                }
            }

            ++index;
        };
    };
}