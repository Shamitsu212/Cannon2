#include "projectile_physics.h"
#include <cmath>

bool checkHit(const Projectile& p, const Target& t) {
    double dx = p.x - t.x;
    double dy = p.y - t.y;
    return std::sqrt(dx*dx + dy*dy) <= t.radius;
}

void updateProjectile(Projectile& p, double dt, double gravity) {
    if (!p.active) return;

    p.x += p.vx * dt;
    p.y += p.vy * dt;
    p.vy += gravity * dt;
}
