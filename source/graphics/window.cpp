#include "graphics/window.h"
#include <iostream>

void vulkan::window::background(core::thread *bt)
{
	XEvent event;
	XNextEvent(display, &event);

	switch(event.type)
	{
		case Expose:
			break;

		case KeyPress:
		{
			char temp[32];
			KeySym ignore;
			Status return_status;
			XKeyPressedEvent pressed = event.xkey;
	
			int count = Xutf8LookupString(xic, &pressed, &temp[0], 32, &ignore, &return_status);
			if(count > 0)
			{
				for(int i = 0; i < count; ++i)
				{
					data.set(temp[i]);
				}
			}

			break;
		}
		case ConfigureNotify:
			std::cout << "ConfigureNotify(move or resize)\r\n";
			break;

		case DestroyNotify:
			std::cout << "Destroy\r\n";
			break;

		case ClientMessage:
			std::cout << "ClientMessage\r\n";
			is_terminated = true;
			break;
	}

	sleep(20);
}

void vulkan::window::reset(int _width, int _height)
{
    init = false; cleanup();

	is_terminated = false;

    width = _width; height = _height;

    init = createWindow(0);
}

bool vulkan::window::createWindow(uint32_t index)
{
    int x = 0, y = 0;

	XInitThreads();

	display = XOpenDisplay(NULL);

	xim = XOpenIM(display, 0, 0, 0);
    xic = XCreateIC(xim, XNInputStyle, XIMPreeditNothing | XIMStatusNothing, NULL);

	windowAttrib.border_pixel = BlackPixel(display, (int)index);
	windowAttrib.background_pixel = WhitePixel(display, (int)index);
	windowAttrib.override_redirect = True;
	windowAttrib.colormap = XCreateColormap(display, RootWindow(display, (int)index),
    DefaultVisual(display, index), AllocNone);
	windowAttrib.event_mask = ExposureMask;

	context = XCreateWindow(display, RootWindow(display, (int)index),
                           x, y, width, height, 0,
                           DefaultDepth(display, index),
                           CopyFromParent | InputOutput,
                           DefaultVisual(display, index),
                           CWBackPixel | CWColormap | CWBorderPixel | CWEventMask,
                           &windowAttrib);

	Atom destroyWindow = XInternAtom(display, "WM_DELETE_WINDOW", 0);
	XSetWMProtocols(display, context, &destroyWindow, 1);

	XSelectInput(display, context, ExposureMask | StructureNotifyMask | KeyPressMask);

	XMapWindow(display, context);

	char caption[20] = "Screen\0";

    XStoreName(display, context, caption);

	return true;
}

void vulkan::window::makeNull()
{
    display = NULL;
}

void vulkan::window::cleanup()
{
    if (display != NULL)
	{
		XFreeColormap(display, windowAttrib.colormap);
		XDestroyWindow(display, context);
		XCloseDisplay(display);
	}
}