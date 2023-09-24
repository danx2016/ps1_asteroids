#ifndef _BULLET_SHIP_H
#define _BULLET_SHIP_H

#include "vec2.h"

extern void bullet_ship_spawn(Vec2 *position, fixed angle, fixed speed);
extern void bullet_ship_reset(Entity* bullet_ship, Vec2 *position, fixed angle, fixed speed);
extern void bullet_ship_fixed_update(Entity* bullet_ship);

#endif /* _BULLET_SHIP_H */