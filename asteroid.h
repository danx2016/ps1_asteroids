#ifndef _ASTEROID_H
#define _ASTEROID_H

#include "entity.h"

#define ASTEROID_NUM_RANDOM_SHAPES 30

typedef enum 
{ 
    SIZE_SMALL  = 10, 
    SIZE_MEDIUM = 20, 
    SIZE_LARGE  = 30
} ASTEROID_SIZE;

#define SCORE_POINTS_ASTEROID_SMALL   25
#define SCORE_POINTS_ASTEROID_MEDIUM  50
#define SCORE_POINTS_ASTEROID_LARGE  100

extern Entity *asteroid_tag_start;
extern Entity *asteroid_tag_end;

extern void asteroid_init();

extern void asteroid_spawn(ASTEROID_SIZE asteroid_size, Vec2 *position, fixed angle);

extern void asteroid_reset(Entity* asteroid, ASTEROID_SIZE asteroid_size, Vec2 *position, fixed angle);

extern void asteroid_hit(Entity* asteroid);
extern void asteroid_fixed_update(Entity* asteroid);

#endif /* _ASTEROID_H */