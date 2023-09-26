#include <stddef.h>
#include <stdint.h>
#include <rand.h>

#include "mem.h"
#include "game.h"
#include "audio.h"
#include "ship.h"
#include "asteroid.h"
#include "explosion.h"

Entity *asteroid_tag_start;
Entity *asteroid_tag_end;

static Polygon *polygons_small[ASTEROID_NUM_RANDOM_SHAPES];
static Polygon *polygons_medium[ASTEROID_NUM_RANDOM_SHAPES];
static Polygon *polygons_large[ASTEROID_NUM_RANDOM_SHAPES];

static Polygon* asteroid_create_shape(ASTEROID_SIZE asteroid_size)
{
    Polygon *polygon = gfx_create_polygon(12);
    int32_t angle_div = 4096 / 12;
    size_t point_index = 0;
    for (int32_t angle = 0; angle < 4096; angle += angle_div)
    {
        int32_t deformation = asteroid_size / 5;
        int32_t radius = (asteroid_size - (2 * deformation) - (rand() % deformation)) << 12;
        Vec2 point = vec2_init(radius, 0);
        point = vec2_rotate(angle, &point); 
        polygon->points[point_index].vx = (int16_t) (point.x >> 12);
        polygon->points[point_index].vy = (int16_t) (point.y >> 12);
        point_index++;
    }
    return polygon;
}

static Entity* asteroid_create_tag(ENTITY_TYPE tag_type)
{
    Entity *tag = entity_get_from_cache();
    tag->type = tag_type;
    tag->is_enabled = false;
    tag->is_destroyed = false;
    tag->previous_entity = NULL;
    tag->next_entity = NULL;
    return tag;
}

void asteroid_init()
{
    asteroid_tag_start = asteroid_create_tag(TAG_START);
    entity_add(asteroid_tag_start);

    asteroid_tag_end = asteroid_create_tag(TAG_END);
    entity_add(asteroid_tag_end);

    // pre-create asteroid random shapes
    for (size_t i = 0; i < ASTEROID_NUM_RANDOM_SHAPES; i++)
    {
        polygons_small[i] = asteroid_create_shape(SIZE_SMALL);
        polygons_medium[i] = asteroid_create_shape(SIZE_MEDIUM);
        polygons_large[i] = asteroid_create_shape(SIZE_LARGE);
    }
}

void asteroid_spawn(ASTEROID_SIZE asteroid_size, Vec2 *position, fixed angle)
{
    Entity *asteroid = entity_get_from_cache();
    asteroid_reset(asteroid, asteroid_size, position, angle);
    entity_add_after(asteroid_tag_start, asteroid);
}

void asteroid_reset(Entity* asteroid, ASTEROID_SIZE asteroid_size, Vec2 *position, fixed angle)
{
    // asteroid's attributes
    asteroid->type = ASTEROID;
    asteroid->is_enabled = true;
    asteroid->is_destroyed = false;

    asteroid->is_invincible = false;
    asteroid->invincible_time = 0;

    asteroid->position = *position;
    asteroid->velocity = vec2_init(1024 + (rand() % 4096), 0);
    asteroid->velocity = vec2_rotate(angle, &asteroid->velocity);

    asteroid->acceleration = vec2_init(0, 0);
    asteroid->angle = 0;
    asteroid->angular_speed = 4 + (32 - (rand() % 64));
    
    asteroid->radius_collision = asteroid_size - 5;
    asteroid->radius_wrap = asteroid_size;

    // asteroid's random shape
    size_t random_shape = rand() % ASTEROID_NUM_RANDOM_SHAPES;
    switch (asteroid_size)
    {
        case SIZE_SMALL: 
            asteroid->polygon = polygons_small[random_shape];
            break;
        case SIZE_MEDIUM:
            asteroid->polygon = polygons_medium[random_shape];
            break;
        case SIZE_LARGE:
            asteroid->polygon = polygons_large[random_shape];
            break;
    }

    // asteroid's 'methods'
    asteroid->hit = asteroid_hit;
    asteroid->fixed_update = asteroid_fixed_update;
    asteroid->render = entity_render;
    asteroid->destroy = entity_destroy;
}

static void asteroid_spawn_random_3(Entity* asteroid, ASTEROID_SIZE size)
{
    fixed angle = rand() % 4096;
    fixed angle_div = 4096 / 3;
    asteroid_spawn(size, &asteroid->position, angle);
    asteroid_spawn(size, &asteroid->position, angle + angle_div);
    asteroid_spawn(size, &asteroid->position, angle + angle_div * 2);
}

void asteroid_hit(Entity* asteroid)
{
    if (asteroid->is_destroyed)
    {
        return;
    }
    
    if (asteroid->radius_wrap == SIZE_LARGE)
    {
        asteroid_spawn_random_3(asteroid, SIZE_MEDIUM);
        game_add_score(SCORE_POINTS_ASTEROID_LARGE);
    }
    else if (asteroid->radius_wrap == SIZE_MEDIUM)
    {
        asteroid_spawn_random_3(asteroid, SIZE_SMALL);
        game_add_score(SCORE_POINTS_ASTEROID_MEDIUM);
    }
    else if (asteroid->radius_wrap == SIZE_SMALL)
    {
        game_add_score(SCORE_POINTS_ASTEROID_SMALL);
    }

    asteroid->destroy(asteroid);
    explosion_spawn(10, &asteroid->position, 20);
    audio_play_sound(SOUND_ID_EXPLOSION);
}

void asteroid_fixed_update(Entity* asteroid)
{
    // pause asteroids while in ready state
    Entity *ship = entity_find_first_type(SHIP);
    if (ship != NULL && ship->is_invincible)
    {
        return;
    }

    asteroid->angle += asteroid->angular_speed;

    // update position
    asteroid->position = vec2_add(&asteroid->position, &asteroid->velocity);

    entity_wrap_position(asteroid);

    // check hit ship
    if (ship != NULL && entity_check_collision(ship, asteroid))
    {
        ship->hit(ship);
    }
}

