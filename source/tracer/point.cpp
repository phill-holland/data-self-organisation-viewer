#include "tracer/point.h"
#include <sstream>
#include <functional>

void tracer::point::deserialise(std::string source)
{
    if(source.size() < 2) return;
    if(source.front() != '(') return;
    if(source.back() != ')') return;

    clear();

    int *position[] = { &x, &y, &z };

    std::string temp = source.substr(1, source.size() - 2);
    std::stringstream ss(temp);

    int index = 0;
    std::string str;

    while(std::getline(ss, str, ','))
    {
        if(index < 3)
        {
            int temp = std::atoi(str.c_str());
            *position[index++] = temp;  
        }
    }
}