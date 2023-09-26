#ifndef _EXPLOSION_H
#define _EXPLOSION_H

#include <stdint.h>

#include "vec2.h"
#include "entity.h"

extern Entity* explosion_create_spark(Vec2 *position, uint32_t life_time);
extern void explosion_spark_reset(Entity* spark, Vec2 *position, uint32_t life_time);

extern void explosion_spark_fixed_update(Entity* spark);

extern void explosion_spawn(uint32_t num_sparks, Vec2 *position, uint32_t life_time);

#endif /* _EXPLOSION_H */