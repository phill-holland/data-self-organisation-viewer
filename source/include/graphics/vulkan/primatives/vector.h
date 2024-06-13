#ifndef _VULKAN_PRIMATIVE_VECTOR
#define _VULKAN_PRIMATIVE_VECTOR

namespace vulkan
{
    namespace primatives
    {
        class vector4
        {
        public:
            float x,y,z,w;

        public:
            vector4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) 
            { 
                this->x = x; 
                this->y = y; 
                this->z = z; 
                this->w = w;
            }
        };

        class vector3
        {
        public:
            float x,y,z;

        public:
            vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) 
            { 
                this->x = x; 
                this->y = y; 
                this->z = z; 
            }

        public:
            operator vector4() const { return { x, y, z }; }
        };

        class vector2
        {
        public:
            float x,y;
            
        public:            
            vector2(float x = 0.0f, float y = 0.0f) 
            { 
                this->x = x; 
                this->y = y; 
            }

        public:
            operator vector3() const { return { x, y }; }
        };
    };
};

#endif