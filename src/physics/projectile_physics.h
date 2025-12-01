#pragma once
#include "../structures/projectile.h"
#include "../structures/target.h"

bool checkHit(const Projectile& p, const Target& t);
void updateProjectile(Projectile& p, double dt, double gravity);
