#include "graphics/vulkan/primatives/matrix.h"

void vulkan::primatives::matrices::matrix4x4::clear()
{    
    for(int x = 0; x < 4; ++x)
    {
        values[x][0] = 0.0f;
        values[x][1] = 0.0f;
        values[x][2] = 0.0f;
        values[x][3] = 0.0f;
    }
}

void vulkan::primatives::matrices::matrix4x4::copy(const matrix4x4 &source)
{
    for(int x = 0; x < 4; ++x)
    {
        values[x][0] = source.values[x][0];
        values[x][1] = source.values[x][1];
        values[x][2] = source.values[x][2];
        values[x][3] = source.values[x][3];
    }
}
vulkan::primatives::matrices::matrix4x4& vulkan::primatives::matrices::matrix4x4::operator=(const matrix4x4& source)
{		
    this->copy(source);
    return *this;
}

vulkan::primatives::matrices::matrix4x4 vulkan::primatives::matrices::matrix4x4::operator*(const matrix4x4 &r)
{
    matrix4x4 temp;
    
    for(int x = 0; x < 4; ++x)
    {
        temp.values[x][0] += (values[x][0] * r.values[0][0]);
        temp.values[x][0] += (values[x][1] * r.values[1][0]);
        temp.values[x][0] += (values[x][2] * r.values[2][0]);
        temp.values[x][0] += (values[x][3] * r.values[3][0]);

        temp.values[x][1] += (values[x][0] * r.values[0][1]);
        temp.values[x][1] += (values[x][1] * r.values[1][1]);
        temp.values[x][1] += (values[x][2] * r.values[2][1]);
        temp.values[x][1] += (values[x][3] * r.values[3][1]);

        temp.values[x][2] += (values[x][0] * r.values[0][2]);
        temp.values[x][2] += (values[x][1] * r.values[1][2]);
        temp.values[x][2] += (values[x][2] * r.values[2][2]);
        temp.values[x][2] += (values[x][3] * r.values[3][2]);

        temp.values[x][3] += (values[x][0] * r.values[0][3]);
        temp.values[x][3] += (values[x][1] * r.values[1][3]);
        temp.values[x][3] += (values[x][2] * r.values[2][3]);
        temp.values[x][3] += (values[x][3] * r.values[3][3]);
    }
    
    return temp;
}

vulkan::primatives::vector4 vulkan::primatives::matrices::matrix4x4::operator*(const vector4 &r) 
{ 
    return vector4(r.x * get(0,0) + r.y * get(1,0) + r.z * get(2,0) + r.w * get(3,0),
                   r.x * get(0,1) + r.y * get(1,1) + r.z * get(2,1) + r.w * get(3,1),
                   r.x * get(0,2) + r.y * get(1,2) + r.z * get(2,2) + r.w * get(3,2),
                   r.x * get(0,3) + r.y * get(1,3) + r.z * get(2,3) + r.w * get(3,3)); 
}

vulkan::primatives::matrices::projection::projection(const float fov, const float ratio, const float near, const float far)
{
    float t = tanf(fov * 0.5f * M_PI / 180) * near;
    float r = ratio * t;
    float l = -r;
    float b = -t;
    
    values[0][0] = 2 * near / (r - l); 
    values[0][1] = 0; 
    values[0][2] = 0; 
    values[0][3] = 0; 

    values[1][0] = 0; 
    values[1][1] = 2 * near / (t - b); 
    values[1][2] = 0; 
    values[1][3] = 0; 

    values[2][0] = (r + l) / (r - l); 
    values[2][1] = (t + b) / (t - b); 
    values[2][2] = -(far + near) / (far - near); 
    values[2][3] = -1; 

    values[3][0] = 0; 
    values[3][1] = 0; 
    values[3][2] = -2 * far * near / (far - near); 
    values[3][3] = 0; 
}