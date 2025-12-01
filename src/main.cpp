#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include <cmath>
#include <vector>
#include <chrono>
#include <cstring>
#include <iostream>
#include <algorithm>

#include "structures/projectile.h"
#include "structures/explosion.h"
#include "structures/target.h"

#include "physics/projectile_physics.h"
#include "physics/explosion_physics.h"

#include "graphics/draw_circle.h"
#include "graphics/draw_target.h"

int main() {
    Display* dpy = XOpenDisplay(nullptr);
    if (!dpy) { std::cerr << "Cannot open display\n"; return 1; }

    int screen = DefaultScreen(dpy);
    int width = 1000;
    int height = 700;
    int ground_y = height - 30;

    Window root = RootWindow(dpy, screen);
    Window win = XCreateSimpleWindow(dpy, root, 0,0,width,height,1,
                                     BlackPixel(dpy, screen), WhitePixel(dpy, screen));

    XSelectInput(dpy, win, ExposureMask | KeyPressMask);
    XMapWindow(dpy, win);

    GC gc = XCreateGC(dpy, win, 0, nullptr);
    Pixmap buffer = XCreatePixmap(dpy, win, width, height, DefaultDepth(dpy, screen));

    double mortar_x = width / 2.0, mortar_y = ground_y;
    double angle = M_PI/2, power = 200.0;

    std::vector<Projectile> projectiles;
    std::vector<Explosion> explosions;

    Target targetLeft  {150.0, 250.0, 40.0};
    Target targetRight {width - 150.0, 300.0, 40.0};

    auto last = std::chrono::high_resolution_clock::now();

    while(true) {
        while(XPending(dpy) > 0) {
            XEvent ev; XNextEvent(dpy, &ev);
            if(ev.type == KeyPress) {
                KeySym key = XLookupKeysym(&ev.xkey,0);
                if(key==XK_Left) angle-=0.05;
                if(key==XK_Right) angle+=0.05;
                if(key==XK_Up) power+=5;
                if(key==XK_Down) power-=5;
                if(power<50) power=50; if(power>500) power=500;

                if(key==XK_space){
                    Projectile p{mortar_x,mortar_y,cos(angle)*power,-sin(angle)*power,true};
                    projectiles.push_back(p);
                }
            }
        }

        auto now = std::chrono::high_resolution_clock::now();
        double dt = std::chrono::duration<double>(now-last).count();
        last = now;

        for(auto& p: projectiles) {
            updateProjectile(p, dt, 200.0);
            if(!p.active) continue;
            if(p.y>=ground_y){ explosions.push_back({p.x,double(ground_y),0,40.0,true}); p.active=false; }
            if(p.x<0||p.x>width||p.y<0||p.y>height) p.active=false;
            if(checkHit(p,targetLeft)||checkHit(p,targetRight)){ explosions.push_back({p.x,p.y,0,40.0,true}); p.active=false; }
        }

        projectiles.erase(std::remove_if(projectiles.begin(),projectiles.end(),[](const Projectile& p){return !p.active;}),projectiles.end());

        for(auto& e: explosions) updateExplosion(e,dt);
        explosions.erase(std::remove_if(explosions.begin(),explosions.end(),[](const Explosion& e){return !e.active;}),explosions.end());

        XSetForeground(dpy,gc,WhitePixel(dpy,screen));
        XFillRectangle(dpy,buffer,gc,0,0,width,height);
        XSetForeground(dpy,gc,BlackPixel(dpy,screen));
        XDrawLine(dpy,buffer,gc,0,ground_y,width,ground_y);

        for(double t=0;t<3.0;t+=0.05){
            double px=mortar_x+cos(angle)*power*t;
            double py=mortar_y-sin(angle)*power*t+0.5*200.0*t*t;
            if(py>ground_y) break;
            XDrawPoint(dpy,buffer,gc,(int)px,(int)py);
        }

        int bx=mortar_x+cos(angle)*40.0;
        int by=mortar_y-sin(angle)*40.0;
        XDrawLine(dpy,buffer,gc,mortar_x,mortar_y,bx,by);
        fillCircle(dpy,buffer,gc,mortar_x,mortar_y,10);

        drawDoubleTarget(dpy,buffer,gc,targetLeft);
        drawDoubleTarget(dpy,buffer,gc,targetRight);

        for(auto& p: projectiles) fillCircle(dpy,buffer,gc,p.x,p.y,5);
        for(auto& e: explosions) drawCircle(dpy,buffer,gc,e.x,e.y,e.radius);

        char buf[64];
        snprintf(buf,sizeof(buf),"Angle: %.1f deg",angle*180.0/M_PI);
        XDrawString(dpy,buffer,gc,10,20,buf,strlen(buf));
        snprintf(buf,sizeof(buf),"Power: %.1f",power);
        XDrawString(dpy,buffer,gc,10,40,buf,strlen(buf));

        XCopyArea(dpy,buffer,win,gc,0,0,width,height,0,0);
        usleep(16000);
    }

    XFreePixmap(dpy,buffer);
    XCloseDisplay(dpy);
    return 0;
}
