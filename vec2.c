#include "math.h"
#include "vec2.h"

Vec2 vec2_init(fixed x, fixed y)
{
    Vec2 vr = { x, y };
    return vr;
}

Vec2 vec2_add(Vec2 *va, Vec2 *vb)
{
    Vec2 vr;
    vr.x = fixed_add(va->x, vb->x);
    vr.y = fixed_add(va->y, vb->y);
    return vr;
}

Vec2 vec2_sub(Vec2 *va, Vec2 *vb)
{
    Vec2 vr;
    vr.x = fixed_sub(va->x, vb->x);
    vr.y = fixed_sub(va->y, vb->y);
    return vr;
}

Vec2 vec2_scale(fixed s, Vec2 *v)
{
    Vec2 vr;
    vr.x = fixed_mul(s, v->x);
    vr.y = fixed_mul(s, v->y);
    return vr;
}

fixed vec2_squared_length(Vec2 *v)
{
    fixed xx = fixed_mul(v->x, v->x);
    fixed yy = fixed_mul(v->y, v->y);
    fixed xx_add_yy = fixed_add(xx, yy);
    return xx_add_yy;
}

fixed vec2_length(Vec2 *v)
{
    fixed xx = fixed_mul(v->x, v->x);
    fixed yy = fixed_mul(v->y, v->y);
    fixed xx_add_yy = fixed_add(xx, yy);
    return math_sqrt(xx_add_yy);
}

Vec2 vec2_normalize(Vec2 *v)
{
    Vec2 vr = { 0, 0 };
    fixed length = vec2_length(v);
    if (length != 0)
    {
        vr.x = fixed_div(v->x, length);
        vr.y = fixed_div(v->y, length);
    }
    return vr;
}

fixed vec2_dot(Vec2 *va, Vec2 *vb)
{
    fixed abx = fixed_mul(va->x, vb->x);
    fixed aby = fixed_mul(vb->y, vb->y);
    return fixed_add(abx, aby);
}

fixed vec2_cross(Vec2 *va, Vec2 *vb)
{
    fixed axby = fixed_mul(va->x, vb->y); 
    fixed bxay = fixed_mul(vb->x, va->y); 
    return fixed_sub(axby, bxay);
}

fixed vec2_angle(Vec2* v) {
    return math_atan2(v->y, v->x);
}

Vec2 vec2_rotate(fixed angle, Vec2 *v)
{
    Vec2 vr;
    fixed s = math_sin(angle);
    fixed c = math_cos(angle);
    fixed xc = fixed_mul(v->x, c);
    fixed ys = fixed_mul(v->y, s);
    fixed nx = fixed_sub(xc, ys);
    fixed xs = fixed_mul(v->x, s);
    fixed yc = fixed_mul(v->y, c);
    fixed ny = fixed_add(xs, yc);
    vr.x = nx;
    vr.y = ny;
    return vr;
}
