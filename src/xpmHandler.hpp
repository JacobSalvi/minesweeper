#pragma once

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/xpm.h>
#include "../images/base.xpm"
#include "../images/bomb.xpm"
#include "../images/empty.xpm"
#include "../images/flag.xpm"
#include "../images/one.xpm"
#include "../images/two.xpm"
#include "../images/three.xpm"
#include "../images/four.xpm"
#include "../images/five.xpm"
#include "../images/six.xpm"
#include "../images/seven.xpm"
#include "../images/eigth.xpm"

class XpmHandler{
    private:
        // const char* data[];
        Pixmap pattern;
        Pixmap clipper;
        XpmAttributes attributes;
        GC gc;
    
    public:
        XpmHandler(Display* display, Window parentWindow, int screen_num, char* data[]) {
            attributes.color_key = XPM_COLOR;
            attributes.valuemask = XpmColorKey | XpmColorTable;
            int status = XpmCreatePixmapFromData(display, parentWindow, data, &pattern, &clipper, &attributes);
            gc = XCreateGC(display, parentWindow, 0, NULL);

            XSetForeground(display, gc, WhitePixel(display, screen_num));
            XSetBackground(display, gc, BlackPixel(display, screen_num));
            XSetClipMask(display, gc, clipper);
            XSetClipOrigin(display, gc, 0, 0);
        }

        void set_new_xpm(Display* display, Window parentWindow, char* new_data[]){
            int status = XpmCreatePixmapFromData(display, parentWindow, new_data, &pattern, &clipper, &attributes);
            XSetClipMask(display, gc, clipper);
        }

        void draw(Display* display, Window win, int x, int y){
            // TODO: remove the magic numbers
            XCopyArea(display, pattern, win, gc, 0, 0, 30, 30, x, y);
        }
};