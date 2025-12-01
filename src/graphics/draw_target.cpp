#include "draw_target.h"
#include "draw_circle.h"

void drawDoubleTarget(Display* dpy, Drawable dr, GC gc, const Target& t) {
    int inner = int(t.radius * 0.6);

    drawCircle(dpy, dr, gc, t.x, t.y, t.radius);
    drawCircle(dpy, dr, gc, t.x, t.y, inner);
    fillCircle(dpy, dr, gc, t.x, t.y, 8);
}
