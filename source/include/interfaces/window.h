#include <X11/Xlib.h>

#ifndef _INTERFACE_INTERFACE_WINDOW
#define _INTERFACE_INTERFACE_WINDOW

namespace vulkan
{
    namespace interfaces
    {
        class window
        {
        public:
            virtual Display *getDisplay() = 0;
            virtual Window getWindow() = 0;

            virtual int getWidth() = 0;
            virtual int getHeight() = 0;
        };
    };
};

#endif