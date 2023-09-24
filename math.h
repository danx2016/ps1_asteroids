#ifndef _MATH_H
#define _MATH_H

#include "fixed.h"

extern void math_init();
extern fixed math_sin(fixed angle);
extern fixed math_cos(fixed angle);
extern fixed math_sqrt(fixed n);
extern fixed math_atan2(fixed y, fixed x);

#endif /* _MATH_H */