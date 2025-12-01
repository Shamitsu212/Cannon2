#include "draw_circle.h"
#include <X11/Xlib.h>

void drawCircle(Display* dpy, Drawable dr, GC gc, int cx, int cy, int r) {
    XDrawArc(dpy, dr, gc, cx - r, cy - r, r*2, r*2, 0, 360*64);
}

void fillCircle(Display* dpy, Drawable dr, GC gc, int cx, int cy, int r) {
    XFillArc(dpy, dr, gc, cx - r, cy - r, r*2, r*2, 0, 360*64);
}
