#include <X11/X.h>
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <chrono>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "app.hpp"
#include "xpmHandler.hpp"
#include "game.hpp"
#include "field.hpp"
#include "tile.hpp"

// utility function to convert colours found on stack overflow
unsigned long _RGB(int r, int g, int b){
    return b + (g << 8) + (r << 16);
}


// 16, 30, 99
int main(int argc, char const* argv[]){
    // TODO: X11 related stuff
    Display* mydisplay;
    Window baseW;
    XSetWindowAttributes baseat;
    XSizeHints wmsize;
    XWMHints wmhints;
    XTextProperty windowName, iconName;
    XEvent myevent;
    GC gc1, gc2;
    XGCValues myGCValues;
    XColor black, white;
    Pixmap pad;
    char* window_name = "Minesweeper";
    char* icon_name = "Mi";
    int screen_num, done;
    unsigned long valuemask;
    int x, y, dx, dy;
    float ratio;

    // open connection to the server
    mydisplay = XOpenDisplay("");

    // create a top level window
    screen_num = DefaultScreen(mydisplay);
    black.pixel = BlackPixel(mydisplay, screen_num);
    white.pixel = WhitePixel(mydisplay, screen_num);
    baseat.background_pixel = white.pixel;
    baseat.border_pixel = black.pixel;
    baseat.event_mask = ExposureMask | KeyPressMask| ButtonPressMask;
    valuemask = CWBackPixel | CWBorderPixel | CWEventMask;
    baseW = XCreateWindow(mydisplay, RootWindow(mydisplay, screen_num),
        0, 0, 100, 5, 2,
        DefaultDepth(mydisplay, screen_num), InputOutput,
        DefaultVisual(mydisplay, screen_num),
        valuemask, &baseat);
    
    // give the window manager hints
    wmsize.flags = USPosition | USSize;
    XSetWMNormalHints(mydisplay, baseW, &wmsize);
    wmhints.initial_state = NormalState;
    wmhints.flags = StateHint;
    XSetWMHints(mydisplay, baseW, &wmhints);
    XStringListToTextProperty(&window_name, 1, &windowName);
    XSetWMName(mydisplay, baseW, &windowName);
    XStringListToTextProperty(&icon_name, 1, &iconName);
    XSetWMIconName(mydisplay, baseW, &iconName);

    // establish window resources
    valuemask = GCForeground | GCBackground;
    myGCValues.background = white.pixel;
    myGCValues.foreground = black.pixel;
    gc1 = XCreateGC(mydisplay, baseW, valuemask, &myGCValues);

    // map the window
    XMapWindow(mydisplay, baseW);

    // application
    application app;
    app.disp = mydisplay;
    app.win = baseW;
    app.gc = gc1;

    GC imageGC = XCreateGC(app.disp, app.win, 0, NULL);

    // CONSTs
    const int height = 16;
    const int width = 30;
    const int bombs = 99;
    const int tile_size = 30;

    Game game = Game(width, height, bombs);
    XResizeWindow(app.disp, app.win, width*tile_size, height*tile_size);
    // game.debug();

    XpmHandler xpm = XpmHandler(app.disp, app.win, screen_num, base_xpm);

    // enter the event loop
    done = 0;
    std::vector<std::vector<int>> a;
    auto start = std::chrono::high_resolution_clock::now();
    std::chrono::system_clock::time_point end;
    unsigned int keyCode;

    // magic
    // consider using two threads, one for the timer and one to handle events
    // we aren't in the 1980s anymore CPU have multiple cores now.
    while (1){
        end = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() > 3000){
            start = end;
        }
        while (XPending(mydisplay)){
            XNextEvent(mydisplay, &myevent);
            switch (myevent.type) {
                case Expose:
                    XSetBackground(mydisplay, imageGC, _RGB(255, 0, 0));
                    XSetForeground(mydisplay, imageGC, BlackPixel(mydisplay, screen_num));
                    game.draw(app, xpm);
                    break;
                case ButtonPress:
                    game.handle_click(myevent.xbutton.x, myevent.xbutton.y, myevent.xbutton.button);
                    game.draw(app, xpm);
                    break;
            }
        }
    }

    // Clean up before exiting
    XUnmapWindow(mydisplay, baseW);
    XDestroyWindow(mydisplay, baseW);
    XCloseDisplay(mydisplay);
    return 0;
}