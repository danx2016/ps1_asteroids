#ifndef _VEC2_H
#define _VEC2_H

#include "fixed.h"

typedef struct 
{
    fixed x;
    fixed y;
} Vec2;

extern Vec2 vec2_init(fixed x, fixed y);
extern Vec2 vec2_add(Vec2 *va, Vec2 *vb);
extern Vec2 vec2_sub(Vec2 *va, Vec2 *vb);
extern Vec2 vec2_scale(fixed s, Vec2 *v);
extern fixed vec2_squared_length(Vec2 *v);
extern fixed vec2_length(Vec2 *v);
extern Vec2 vec2_normalize(Vec2 *v);
extern fixed vec2_dot(Vec2 *va, Vec2 *vb);
extern fixed vec2_cross(Vec2 *va, Vec2 *vb);
extern fixed vec2_angle(Vec2* v);
extern Vec2 vec2_rotate(fixed angle, Vec2 *v);

#endif /* _VEC2_H */