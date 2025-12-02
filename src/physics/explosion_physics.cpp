#include "explosion_physics.h"

// Обновление анимации взрыва
void updateExplosion(Explosion& e, double dt) {
    // Если взрыв уже завершён end
    if (!e.active) return;

    e.radius += 200.0 * dt;

    // Если радиус достиг максимума — считаем взрыв завершённым
    if (e.radius >= e.max_radius)
        e.active = false;
}
