#include <stddef.h>
#include <stdio.h>

#include "mem.h"
#include "entity.h"

static Entity root_entity;
static Entity* last_entity;

static Entity destroyed_root_entity;

static void entity_save_to_cache(Entity *entity)
{
    Entity* root_next = destroyed_root_entity.next_entity;
    destroyed_root_entity.next_entity = entity;
    entity->previous_entity = &destroyed_root_entity;
    if (root_next == NULL)
    {
        entity->next_entity = NULL;
    }
    else
    {
        entity->next_entity = root_next;
        root_next->previous_entity = entity;
    }
}

void entity_init()
{
    root_entity.type = ROOT;
    root_entity.next_entity = NULL;
    root_entity.previous_entity = NULL;

    last_entity = &root_entity;

    destroyed_root_entity.type = ROOT;
    destroyed_root_entity.next_entity = NULL;
    destroyed_root_entity.previous_entity = NULL;

    // create initial cached entities
    for (int i = 0; i < ENTITY_INITIAL_CACHE_SIZE; i++)
    {
        Entity *entity = entity_create();
        entity_save_to_cache(entity);
    }
}

Entity* entity_create()
{
    Entity *entity = mem_alloc(sizeof(Entity));
    return entity;
}

Entity* entity_get_from_cache()
{
    if (destroyed_root_entity.next_entity == NULL)
    {
        return entity_create();
    }
    else
    {
        Entity* free = destroyed_root_entity.next_entity;
        Entity* next = free->next_entity;
        destroyed_root_entity.next_entity = NULL;
        if (next != NULL)
        {
            destroyed_root_entity.next_entity = next;
            next->previous_entity = &destroyed_root_entity;
        }
        return free;
    }
}

void entity_add(Entity *entity)
{
    last_entity->next_entity = entity;
    entity->previous_entity = last_entity;
    entity->next_entity = NULL;
    last_entity = entity;
}

void entity_add_after(Entity *entity_after, Entity *entity)
{
    Entity *next = entity_after->next_entity;
    entity_after->next_entity = entity;
    entity->previous_entity = entity_after;
    entity->next_entity = NULL;
    if (next != NULL)
    {
        entity->next_entity = next;
        next->previous_entity = entity;
    }

    if (entity_after == last_entity)
    {
        last_entity = entity;
    }
}

void entity_remove(Entity *entity)
{
    Entity *previous = entity->previous_entity;
    Entity *next = entity->next_entity;

    if (previous != NULL)
    {
        previous->next_entity = next;
    }

    if (next != NULL)
    {
        next->previous_entity = previous;
    }

    if (last_entity == entity)
    {
        last_entity = previous;
    }

    // save the destroyed entity to cache so it can 
    // be reused later using entity_get_from_cache()
    entity_save_to_cache(entity);
}

void entity_remove_all()
{
    Entity *current_entity = root_entity.next_entity;
    while (current_entity != NULL)
    {
        Entity *destroy_entity = current_entity;
        current_entity = current_entity->next_entity;
        if (destroy_entity->type != ROOT &&
            destroy_entity->type != TAG_START &&
            destroy_entity->type != TAG_END)
        {
            destroy_entity->is_destroyed = true;
            entity_remove(destroy_entity);
        }
    }
}

Entity* entity_find_first_type(ENTITY_TYPE type)
{
    Entity *current_entity = root_entity.next_entity;
    while (current_entity != NULL)
    {
        if (current_entity->type == type)
        {
            return current_entity;
        }
        current_entity = current_entity->next_entity;
    }
    return NULL;
}

void entity_update_all()
{
    Entity *current_entity = root_entity.next_entity;
    while (current_entity != NULL)
    {
        if (current_entity->is_enabled)
        {
            current_entity->fixed_update(current_entity);
        }
        Entity *destroy_entity = current_entity;
        current_entity = current_entity->next_entity;
        if (destroy_entity->is_destroyed)
        {
            entity_remove(destroy_entity);
        }
    }
}

void entity_render_all()
{
    Entity *current_entity = root_entity.next_entity;
    while (current_entity != NULL)
    {
        if (current_entity->is_enabled)
        {
            current_entity->render(current_entity);
        }
        current_entity = current_entity->next_entity;
    }
}

void entity_render(Entity* entity)
{
    if (!entity->is_invincible || (entity->is_invincible && (entity->invincible_time % 4) < 2))
    {
        int16_t entity_x = (int16_t) (entity->position.x >> 12);
        int16_t entity_y = (int16_t) (entity->position.y >> 12);
        gfx_draw_polygon(entity->polygon, entity_x, entity_y, entity->angle);
    }
}

void entity_destroy(Entity* entity)
{
    entity->is_destroyed = true;
}

void entity_wrap_position(Entity *entity)
{
    fixed er = entity->radius_wrap;
    fixed entity_min_x = (-er) << 12;
    fixed entity_max_x = (gfx_screen_width + er) << 12;
    fixed entity_min_y = (gfx_screen_offset_y - er) << 12;
    fixed entity_max_y = (gfx_screen_offset_y + gfx_screen_height + er) << 12;

    if (entity->position.x > entity_max_x)
    {
        entity->position.x = entity_min_x;
    }

    if (entity->position.x < entity_min_x)
    {
        entity->position.x = entity_max_x;
    }

    if (entity->position.y > entity_max_y)
    {
        entity->position.y = entity_min_y;
    }

    if (entity->position.y < entity_min_y)
    {
        entity->position.y = entity_max_y;
    }    
}

bool entity_is_out_of_display_area(Entity *entity)
{
    fixed er = entity->radius_wrap;
    fixed entity_min_x = (-er) << 12;
    fixed entity_max_x = (gfx_screen_width + er) << 12;
    fixed entity_min_y = (gfx_screen_offset_y - er) << 12;
    fixed entity_max_y = (gfx_screen_offset_y + gfx_screen_height + er) << 12;
    
    if (entity->position.x > entity_max_x) return true;
    if (entity->position.x < entity_min_x) return true;
    if (entity->position.y > entity_max_y) return true;
    if (entity->position.y < entity_min_y) return true;

    return false;
}

bool entity_check_collision(Entity *entity_a, Entity *entity_b)
{
    Vec2 dif = vec2_sub(&entity_b->position, &entity_a->position);
    fixed sq_len = vec2_squared_length(&dif);
    fixed rad_sum = (entity_a->radius_collision + entity_b->radius_collision) << 12;
    fixed sq_rad_sum = fixed_mul(rad_sum, rad_sum);
    return sq_len <= sq_rad_sum;
}