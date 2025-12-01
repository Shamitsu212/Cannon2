#include "explosion_physics.h"

void updateExplosion(Explosion& e, double dt) {
    if (!e.active) return;

    e.radius += 200.0 * dt;
    if (e.radius >= e.max_radius)
        e.active = false;
}
