#include <stddef.h>
#include <stdio.h>

#include "mem.h"
#include "entity.h"
#include "asteroid.h"
#include "bullet_ship.h"

static Polygon* bullet_ship_polygon = NULL;

void bullet_ship_spawn(Vec2 *position, fixed angle, fixed speed)
{
    Entity *bullet_ship = entity_get_from_cache();
    bullet_ship_reset(bullet_ship, position, angle, speed);
    entity_add(bullet_ship);
}

static Polygon* explosion_create_spark_shape()
{
    Polygon *polygon = gfx_create_polygon(4);
    setVector(&polygon->points[0], -1, -1, 0);
    setVector(&polygon->points[1], -1, 1, 0);
    setVector(&polygon->points[2], 1, 1, 0);
    setVector(&polygon->points[3], 1, -1, 0);
    return polygon;
}

void bullet_ship_reset(Entity* bullet_ship, Vec2 *position, fixed angle, fixed speed)
{
    // bullet ship's attributes
    bullet_ship->type = BULLET_SHIP;
    bullet_ship->is_enabled = true;
    bullet_ship->is_destroyed = false;

    bullet_ship->is_invincible = false;
    bullet_ship->invincible_time = 0;

    bullet_ship->radius_collision = 2;
    bullet_ship->radius_wrap = 2;
    
    bullet_ship->velocity = vec2_init(speed, 0);
    bullet_ship->velocity = vec2_rotate(angle, &bullet_ship->velocity);
    
    bullet_ship->position = *position;
    bullet_ship->position = vec2_add(&bullet_ship->position, &bullet_ship->velocity);

    bullet_ship->acceleration = vec2_init(0, 0);
    bullet_ship->angle = 0;
    bullet_ship->angular_speed = 0;
    
    // bullet ship's shape
    if (bullet_ship_polygon == NULL)
    {
        bullet_ship_polygon = explosion_create_spark_shape();
    }
    bullet_ship->polygon = bullet_ship_polygon;

    // bullet ship's 'methods'
    bullet_ship->hit = NULL;
    bullet_ship->fixed_update = bullet_ship_fixed_update;
    bullet_ship->render = entity_render;
    bullet_ship->destroy = entity_destroy;
}

void bullet_ship_fixed_update(Entity* bullet_ship)
{
    bullet_ship->angle += bullet_ship->angular_speed;

    // update position
    bullet_ship->position = vec2_add(&bullet_ship->position, &bullet_ship->velocity);

    if (entity_is_out_of_display_area(bullet_ship))
    {
        bullet_ship->destroy(bullet_ship);
    }

    // check hit asteroids
    Entity *next_asteroid = asteroid_tag_start->next_entity;
    while (next_asteroid != NULL && next_asteroid != asteroid_tag_end)
    {
        if (entity_check_collision(bullet_ship, next_asteroid))
        {
            next_asteroid->hit(next_asteroid);
            bullet_ship->destroy(bullet_ship);
            break;
        }
        next_asteroid = next_asteroid->next_entity;
    }
}

