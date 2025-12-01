#pragma once
#include <X11/Xlib.h>

void drawCircle(Display* dpy, Drawable dr, GC gc, int cx, int cy, int r);
void fillCircle(Display* dpy, Drawable dr, GC gc, int cx, int cy, int r);
