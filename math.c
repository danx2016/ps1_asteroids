#include <libgte.h>

#include "math.h"

void math_init()
{
    InitGeom();
}

fixed math_sin(fixed angle)
{
    angle = angle % ONE;
    if (angle < 0)
    {
        angle = angle + ONE;
    }
    fixed q = angle / 1024;
    fixed sign = q > 1 ? -1 : 1;
    if ((q % 2) == 0)
    {
        return sign * rsin(angle % 1024);
    }
    return sign * rsin(1024 - (angle % 1024));
}

fixed math_cos(fixed angle)
{
    return math_sin(angle + 1024);
}

fixed math_sqrt(fixed n)
{
    return csqrt(n);
}

fixed math_atan2(fixed y, fixed x)
{
    return ratan2(y, x);
}

