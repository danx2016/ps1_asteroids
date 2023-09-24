#ifndef _FIXED_H
#define _FIXED_H

#include <stdint.h>

// fixed point <1,19,12> type
typedef int32_t fixed;

#define fixed_add(a, b) (a + b)
#define fixed_sub(a, b) (a - b)
#define fixed_mul(a, b) ((int32_t) (((int64_t) a * b) >> 12))
#define fixed_div(a, b) ((int32_t) (((int64_t) a << 12) / b))

#endif /* _FIXED_H */