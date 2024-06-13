#include <X11/Xlib.h>
#include <stdint.h>
#include <atomic>
#include "interfaces/window.h"
#include "core/thread.h"
#include "core/fifo.h"

#ifndef _VULKAN_WINDOW
#define _VULKAN_WINDOW

namespace vulkan
{
    class window : public interfaces::window, public core::thread
    {
        XSetWindowAttributes windowAttrib;
        Display *display;
        Window context;

        XIM xim;
        XIC xic;

        core::queue::fifo<char, 255> data;

        std::atomic<bool> is_terminated;        

        int width, height;

        bool init;

	protected:
		void background(core::thread *bt);

    public:
        window(int _width, int _height) { makeNull(); reset(_width, _height); }
        ~window() { cleanup(); }

        bool initalised() { return init; }
        void reset(int _width, int _height);

        int getWidth() { return width; } 
        int getHeight() { return height; }
        
        bool terminated() { return is_terminated; }

        bool keypressed(char &result)
        {
            return data.get(result);
        }

    public:
        Display *getDisplay() { return display; } 
        Window getWindow() { return context; }

    protected:
        bool createWindow(uint32_t index);

    protected:
        void makeNull();
        void cleanup();
    };
};

#endif