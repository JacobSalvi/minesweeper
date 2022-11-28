#pragma once

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

struct application{
    Display* disp;
    Window win;
    GC gc;
};