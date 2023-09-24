#include <stddef.h>
#include <rand.h>
#include <libgte.h>

#include "mem.h"
#include "gfx.h"
#include "audio.h"
#include "input.h"
#include "ship.h"
#include "bullet_ship.h"
#include "asteroid.h"
#include "explosion.h"

Entity *ship_tag_start;
static Polygon* ship_polygon = NULL;

static Polygon* ship_create_shape()
{
    Polygon *polygon = gfx_create_polygon(7);
    setVector(&polygon->points[0], -10, -5, 0);
    setVector(&polygon->points[1], -10, 5, 0);
    setVector(&polygon->points[2], 10, 0, 0);
    setVector(&polygon->points[3], -10, -5, 0);
    // propulsion
    setVector(&polygon->points[4], -10, -2, 0);
    #define SHIP_POLYGON_PROPULSION_INDEX 5
    setVector(&polygon->points[SHIP_POLYGON_PROPULSION_INDEX], -10, 0, 0);
    setVector(&polygon->points[6], -10, 2, 0);
    return polygon;
}

void ship_init()
{
    ship_tag_start = entity_get_from_cache();
    ship_tag_start->type = TAG_START;
    ship_tag_start->is_enabled = false;
    ship_tag_start->is_destroyed = false;
    ship_tag_start->previous_entity = NULL;
    ship_tag_start->next_entity = NULL;
    entity_add(ship_tag_start);
}

Entity* ship_create()
{
    Entity *ship = entity_get_from_cache();
    ship_reset(ship);
    return ship;
}

void ship_reset(Entity* ship)
{
    // ship's attributes
    ship->type = SHIP;
    ship->is_enabled = true;
    ship->is_destroyed = false;

    ship->is_invincible = true;
    ship->invincible_time = 180;

    ship->radius_collision = 8;
    ship->radius_wrap = 10;

    ship->position = vec2_init(160 << 12, 128 << 12);
    ship->velocity = vec2_init(0, 0);
    ship->acceleration = vec2_init(0, 0);
    ship->angle = 0;
    ship->angular_speed = 0;

    // ship's shape
    if (ship_polygon == NULL)
    {
        ship_polygon = ship_create_shape();
    }    
    ship->polygon = ship_polygon;

    // ship's 'methods'
    ship->hit = ship_hit;
    ship->fixed_update = ship_fixed_update;
    ship->render = entity_render;
    ship->destroy = entity_destroy;
}

void ship_hit(Entity* ship)
{
    if (ship->is_invincible)
    {
        return;
    }

    if (!ship->is_destroyed)
    {
        ship->destroy(ship);
        explosion_spawn(10, &ship->position, 20);
        audio_play_sound(SOUND_ID_EXPLOSION);
    }
}

void ship_fixed_update(Entity* ship)
{
    // propulsion
    ship->polygon->points[SHIP_POLYGON_PROPULSION_INDEX].vx = -10;

    // pause ship when it's in ready state
    if (ship->is_invincible)
    {
        ship->invincible_time--;
        if (ship->invincible_time <= 0)
        {
            ship->invincible_time = 0;
            ship->is_invincible = false;
        }
        return;
    }

    // update angle
    if (input_is_action_pressed(0, LEFT))
    {
        ship->angle -= SHIP_ANGULAR_SPEED;
    }

    if (input_is_action_pressed(0, RIGHT))
    {
        ship->angle += SHIP_ANGULAR_SPEED;
    }

    // update acceleration    
    ship->acceleration = vec2_init(0, 0);

    if (input_is_action_pressed(0, UP))
    {
        ship->acceleration = vec2_init(256, 0);
        ship->acceleration = vec2_rotate(ship->angle, &ship->acceleration);

        // propulsion
        ship->polygon->points[SHIP_POLYGON_PROPULSION_INDEX].vx = -15 - (rand() % 10);
    }

    // update velocity
    ship->velocity = vec2_add(&ship->velocity, &ship->acceleration);
    
    // limit max linear velocity
    if (vec2_length(&ship->velocity) > SHIP_MAX_LINEAR_SPEED)
    {
        ship->velocity = vec2_normalize(&ship->velocity);
        ship->velocity = vec2_scale(SHIP_MAX_LINEAR_SPEED, &ship->velocity);
    }

    // update position
    ship->position = vec2_add(&ship->position, &ship->velocity);

    entity_wrap_position(ship);

    // ship->velocity *= 0.99
    // ship->velocity = vec2_scale(4055, &ship->velocity);

    // shot
    bool can_shoot = !ship->is_invincible && asteroid_tag_start->next_entity != asteroid_tag_end;
    if (can_shoot && input_is_action_just_pressed(0, CONFIRM))
    {
        bullet_ship_spawn(&ship->position, ship->angle, 4 * ONE);
        audio_play_sound(SOUND_ID_SHOOT);
    }
}
