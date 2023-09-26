#ifndef _GFX_H
#define _GFX_H

#include <stdint.h>
#include <libgpu.h>

#include "fixed.h"

#define PRIMITIVES_BUFFER_SIZE 32768

extern int16_t gfx_screen_width;
extern int16_t gfx_screen_height;
extern int16_t gfx_screen_offset_y;

extern void gfx_init();
extern void gfx_swap_buffers();

typedef struct
{
    uint32_t num_points;
    SVECTOR points[];
} Polygon;

extern Polygon* gfx_create_polygon(uint8_t num_points);
extern void gfx_draw_polygon(Polygon *polygon, int16_t x, int16_t y, fixed angle);

extern void gfx_print(uint8_t *text);

#endif /* _GFX_H */