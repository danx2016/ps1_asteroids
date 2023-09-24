#ifndef _ENTITY_H
#define _ENTITY_H

#include <stdbool.h>

#include "vec2.h"
#include "gfx.h"

#define ENTITY_INITIAL_CACHE_SIZE 256

typedef enum 
{
    ROOT,
    TAG_START,
    TAG_END,
    SHIP,
    BULLET_SHIP,
    ASTEROID,
    SPARK
} ENTITY_TYPE;

typedef struct Entity Entity;

struct Entity
{
    ENTITY_TYPE type;
    bool is_enabled;
    bool is_destroyed;
    
    bool is_invincible;
    uint32_t invincible_time;
    
    uint32_t life_time;

    //used for circle-circle collision detection
    int32_t radius_collision;

    // used to wrap position of this entity
    int32_t radius_wrap;

    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;
    fixed angle;
    fixed angular_speed;

    Polygon *polygon;

    void (*hit)(Entity *entity);
    void (*fixed_update)(Entity *entity);
    void (*render)(Entity *entity);
    void (*destroy)(Entity *entity);

    Entity *previous_entity;
    Entity *next_entity;
};

extern void entity_init();

extern Entity* entity_create();
extern Entity* entity_get_from_cache();

extern void entity_add(Entity *entity);
extern void entity_add_after(Entity *entity_after, Entity *entity);

extern void entity_remove(Entity *entity);
extern void entity_remove_all();

extern Entity* entity_find_first_type(ENTITY_TYPE type);

extern void entity_update_all();

extern void entity_render(Entity* entity);
extern void entity_render_all();

extern void entity_destroy(Entity* entity);

extern void entity_wrap_position(Entity *entity);
extern bool entity_is_out_of_display_area(Entity *entity);
extern bool entity_check_collision(Entity *entity_a, Entity *entity_b);

#endif /* _ENTITY_H */