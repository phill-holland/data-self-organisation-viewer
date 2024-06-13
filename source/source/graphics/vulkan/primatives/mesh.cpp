#include "graphics/vulkan/primatives/mesh.h"
#include <fstream>
#include <sstream>
#include <tuple>

bool vulkan::primatives::mesh::load(std::string filename)
{
    std::vector<std::tuple<vector3,vector3>> temp;    
    std::vector<vector2> uv;     
    std::vector<vector3> vn;       

    std::ifstream source(filename);
    if(source.is_open())
    {
        for(std::string value; getline(source, value); )
        {
            std::stringstream stream(value);
		    std::string type, word;
	
            std::vector<std::string> words;

            if(stream >> type)
            {
                if(type == "v")
                {
                    while(stream >> word) words.push_back(word);

                    if(words.size() >= 3)
                    {
                        vector3 v, c(1.0f,1.0f,1.0f);
                        float *xyz[] = { &v.x, &v.y, &v.z, &c.x, &c.y, &c.z };
                        
                        int index = 0;

                        int length = words.size();
                        if(length > 6) length = 6;

                        for(std::vector<std::string>::iterator it = words.begin(); it < words.begin() + length; ++it)
                        {
                            *xyz[index++] = std::atof((*it).c_str());
                        }

                        temp.push_back(std::tuple<vector3,vector3>(v,c));
                    }
                }
                if(type == "vn")
                {
                    while(stream >> word) words.push_back(word);

                    if(words.size() >= 3)
                    {
                        vector3 v;
                        float *xyz[] = { &v.x, &v.y, &v.z };
                        
                        int index = 0;

                        int length = words.size();
                        if(length > 3) length = 3;

                        for(std::vector<std::string>::iterator it = words.begin(); it < words.begin() + length; ++it)
                        {
                            *xyz[index++] = std::atof((*it).c_str());
                        }

                        vn.push_back(v);
                    }
                }
                else if(type =="f")
                {
                    // f v1/vt1/vn1
                    // vertex, texture, normal

                    while(stream >> word) words.push_back(word);

                    if(words.size() >= 3)
                    {
                        int xyz[] = { 0, 0, 0 };
                        int index = 0;

                        for(std::vector<std::string>::iterator it = words.begin(); it < words.begin() + 3; ++it)
                        {
                            std::stringstream data(*it);
                            for(std::string token; getline(data, token, '/'); )
                            {
                                xyz[index++] = atoi(token.c_str());
                                break;
                            };
                        }
                        
                        if((xyz[0] > 0)&&(xyz[1] > 0)&&(xyz[2] > 0))
                        {
                            std::tuple<vector3,vector3> v1 = temp[xyz[0] - 1];
                            std::tuple<vector3,vector3> v2 = temp[xyz[1] - 1];
                            std::tuple<vector3,vector3> v3 = temp[xyz[2] - 1];
                                                
                            vertices.push_back(vertex(std::get<0>(v1),std::get<1>(v1)));
                            vertices.push_back(vertex(std::get<0>(v2),std::get<1>(v2)));
                            vertices.push_back(vertex(std::get<0>(v3),std::get<1>(v3)));
                        }
                    }
                }
                else if(type == "vt")
                {
                    while(stream >> word) words.push_back(word);

                    if(words.size() >= 2)
                    {
                        vector2 vt;
                        float *xy[] = { &vt.x, &vt.y };
                        
                        int index = 0;

                        int length = words.size();
                        if(length > 2) length = 2;

                        for(std::vector<std::string>::iterator it = words.begin(); it < words.begin() + length; ++it)
                        {
                            *xy[index++] = std::atof((*it).c_str());
                        }

                        uv.push_back(vt);
                    }
                }
            }
        }

        source.close();

        // ****

        int uv_length = uv.size();
        if(uv_length > 0)
        {
            if(uv_length > vertices.size()) uv_length = vertices.size();

            for(int i = 0; i < uv_length; ++i)            
            {
                vertices[i].uv = uv[i];
            }
        }

        // ****

        int vn_length = vn.size();
        if(vn_length > 0)
        {
            if(vn_length > vertices.size()) vn_length = vertices.size();

            for(int i = 0; i < vn_length; ++i)            
            {
                vertices[i].normal = vn[i];
            }
        }

        // ***

        return vertices.size() > 0;
    }

    return false;
}