#pragma once
#include <X11/Xlib.h>
#include "../structures/target.h"

void drawDoubleTarget(Display* dpy, Drawable dr, GC gc, const Target& t);
