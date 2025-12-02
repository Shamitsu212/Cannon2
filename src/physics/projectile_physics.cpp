#include "projectile_physics.h"
#include <cmath>

// Проверка попадания снаряда в цель
bool checkHit(const Projectile& p, const Target& t) {
    double dx = p.x - t.x;
    double dy = p.y - t.y;

    // Если расстояние между центрами меньше радиуса цели — попадание
    return std::sqrt(dx*dx + dy*dy) <= t.radius;
}

// Обновление состояния снаряда
void updateProjectile(Projectile& p, double dt, double gravity) {
    // Если снаряд неактивен end
    if (!p.active) return;

    p.x += p.vx * dt;
    p.y += p.vy * dt;

    p.vy += gravity * dt;
}
