#include <stddef.h>
#include <stdbool.h>
#include <libgpu.h>
#include <libetc.h>

#include "mem.h"
#include "math.h"
#include "gfx.h"

int16_t gfx_screen_width;
int16_t gfx_screen_height;
int16_t gfx_screen_offset_y;

static DISPENV disp[2];
static DRAWENV draw[2];
static uint8_t db;

static uint8_t primitives_buffer[PRIMITIVES_BUFFER_SIZE];
static uint8_t *next_primitive;

#define OT_LEN 1
static uint32_t ot[OT_LEN];

// kindly provided by @spicyjpeg
static bool is_gpu_in_pal_mode(void) {
    return (*((uint32_t *) 0x1f801814) >> 20) & 1;
}

static void gfx_begin_render()
{
    next_primitive = primitives_buffer;
    ClearOTagR(ot, OT_LEN);
}

// initialize screen resolution according to
// the current gpu video mode (pal or ntsc)
void gfx_init()
{
    if (is_gpu_in_pal_mode())
    {
        gfx_screen_width = 320;
        gfx_screen_height = 256;
        gfx_screen_offset_y = 0;
    }
    else
    {
        gfx_screen_width = 320;
        gfx_screen_height = 240;
        gfx_screen_offset_y = 8;
    }
    ResetGraph(0);
    SetDefDispEnv(&disp[0], 0, gfx_screen_offset_y, gfx_screen_width, gfx_screen_height);
    SetDefDispEnv(&disp[1], 0, 256 + gfx_screen_offset_y, gfx_screen_width, gfx_screen_height);
    SetDefDrawEnv(&draw[0], 0, 256, gfx_screen_width, 256);
    SetDefDrawEnv(&draw[1], 0, 0, gfx_screen_width, 256);
    draw[0].isbg = 1;
    draw[1].isbg = 1;
    setRGB0(&draw[0], 0, 0, 0);
    setRGB0(&draw[1], 0, 0, 0);
    SetDispMask(1);

    FntLoad(320, 0);
    FntOpen(8, 0, 320, 240, 0, FNT_MAX_SPRT);

    gfx_begin_render();
}

void gfx_swap_buffers()
{
    FntFlush(-1);
    DrawOTag2(&ot[0]);
    DrawSync(0);
    VSync(0);
    db = !db;
    PutDispEnv(&disp[db]);
    PutDrawEnv(&draw[db]);
    gfx_begin_render();
}

Polygon* gfx_create_polygon(uint8_t num_points)
{
    Polygon *polygon = (Polygon*) mem_alloc(sizeof(Polygon) + num_points * sizeof(SVECTOR));
    polygon->num_points = num_points;
    for (int i = 0; i < num_points; i++)
    {
        polygon->points[i].vx = 0;
        polygon->points[i].vy = 0;
        polygon->points[i].vz = 0;
    }
    return polygon;
}

static void rotate_svector(SVECTOR *v, fixed angle)
{
    fixed s = math_sin(angle);
    fixed c = math_cos(angle);
    int32_t nx = (c * v->vx - s * v->vy) >> 12;
    int32_t ny = (s * v->vx + c * v->vy) >> 12;
    v->vx = (int16_t) nx;
    v->vy = (int16_t) ny;
}

void gfx_draw_polygon(Polygon *polygon, int16_t x, int16_t y, fixed angle)
{
    uint8_t num_points = polygon->num_points;
    for (int i = 0; i < num_points; i++)
    {
        SVECTOR pa = polygon->points[i];
        SVECTOR pb = polygon->points[(i + 1) % num_points];
        rotate_svector(&pa, angle);
        rotate_svector(&pb, angle);
        LINE_F2 *line = (LINE_F2*) next_primitive;
        setLineF2(line);
        setRGB0(line, 255, 255, 255);
        setXY2(line, pa.vx + x, pa.vy + y, pb.vx + x, pb.vy + y);
        AddPrim(&ot[0], line);
        next_primitive += sizeof(LINE_F2);
    }
}

void gfx_print(uint8_t *text)
{
    FntPrint(text);
}