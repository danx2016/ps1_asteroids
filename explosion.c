#include <stddef.h>
#include <rand.h>

#include "gfx.h"
#include "asteroid.h"
#include "explosion.h"

static Polygon* spark_polygon = NULL;

static Polygon* explosion_create_spark_shape()
{
    Polygon *polygon = gfx_create_polygon(4);
    setVector(&polygon->points[0], -1, -1, 0);
    setVector(&polygon->points[1], -1, 1, 0);
    setVector(&polygon->points[2], 1, 1, 0);
    setVector(&polygon->points[3], 1, -1, 0);
    return polygon;
}

Entity* explosion_create_spark(Vec2 *position, uint32_t life_time)
{
    Entity *spark = entity_get_from_cache();
    explosion_spark_reset(spark, position, life_time);
    return spark;
}

void explosion_spark_reset(Entity* spark, Vec2 *position, uint32_t life_time)
{
    // spark's attributes
    spark->type = SPARK;
    spark->is_enabled = true;
    spark->is_destroyed = false;

    spark->is_invincible = true;
    spark->invincible_time = life_time;
    
    spark->life_time = life_time;

    spark->radius_collision = 0;
    spark->radius_wrap = 0;

    spark->position = *position;

    spark->velocity = vec2_init(6144 + (rand() % 4096), 0);
    spark->velocity = vec2_rotate(rand() % 4096, &spark->velocity);

    spark->acceleration = vec2_init(0, 0);

    spark->angle = 0;
    spark->angular_speed = 0;
    
    // spark's shape
    if (spark_polygon == NULL)
    {
        spark_polygon = explosion_create_spark_shape();
    }
    spark->polygon = spark_polygon;

    // spark's 'methods'
    spark->hit = NULL;
    spark->fixed_update = explosion_spark_fixed_update;
    spark->render = entity_render;
    spark->destroy = entity_destroy;
}

void explosion_spark_fixed_update(Entity *spark)
{
    // reuse 'invicible' feature just to blink
    spark->invincible_time--;

    // update position
    spark->position = vec2_add(&spark->position, &spark->velocity);

    // life time
    spark->life_time--;
    if (spark->life_time <= 0)
    {
        spark->destroy(spark);
    }
}

void explosion_spawn(uint32_t num_sparks, Vec2 *position, uint32_t life_time)
{
    for (uint32_t i = 0; i < num_sparks; i++)
    {
        Entity *spark = explosion_create_spark(position, life_time);
        entity_add(spark);
    }
}
