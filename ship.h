#ifndef _SHIP_H
#define _SHIP_H

#include "entity.h"

#define SHIP_MAX_LINEAR_SPEED 12288 // =3.0
#define SHIP_ANGULAR_SPEED 48

extern Entity *ship_tag_start;

extern void ship_init();
extern Entity* ship_create();
extern void ship_reset(Entity* ship);
extern void ship_hit(Entity* ship);
extern void ship_fixed_update(Entity* ship);

#endif /* SHIP */
