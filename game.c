#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <libetc.h>

#include "game.h"
#include "fixed.h"
#include "math.h"
#include "vec2.h"
#include "mem.h"
#include "gfx.h"
#include "audio.h"
#include "input.h"
#include "ship.h"
#include "bullet_ship.h"
#include "asteroid.h"

static bool game_started = false;

static uint32_t game_title_count = 0;
static uint32_t game_cleared_count = 0;
static uint32_t ship_destroyed_count = 0;
static uint32_t go_count = 0;
static uint32_t game_over_count = 0;

static uint32_t score = 0;

static uint8_t lives = 3;

static void reset_ship()
{
    Entity *ship = ship_create();
    entity_add_after(ship_tag_start, ship);
    ship_destroyed_count = 0;
}

static void reset_level()
{
    const ASTEROID_SIZE asteroid_sizes[] = { SIZE_SMALL, SIZE_SMALL, SIZE_LARGE };
    const uint8_t asteroid_nums[] = { 5, 2, 2 };
    for (uint8_t s = 0; s < 3; s++)
    {
        for (uint8_t i = 0; i < asteroid_nums[s]; i++)
        {
            fixed px = (rand() % gfx_screen_width) << 12;
            fixed py = (rand() % gfx_screen_height) << 12;
            Vec2 position = vec2_init(px, py);
            fixed angle = rand() % 4096;
            asteroid_spawn(asteroid_sizes[s], &position, angle);
        }
    }
}

static void start_title()
{
    audio_play_music(MUSIC_ID_TITLE);
    entity_remove_all();
    reset_level();
    game_title_count = 0;
    game_started = false;
    lives = 3;
    score = 0;
}

static void start_new_game()
{
    audio_play_music(MUSIC_ID_PLAYING);
    audio_pause_music();
    audio_play_sound(SOUND_ID_START);
    VSync(60);
    audio_resume_music();
    entity_remove_all();
    reset_ship();
    reset_level();
    game_started = true;
    game_cleared_count = 0;
    game_over_count = 0;
    lives = 3;
    score = 0;
    srand(VSync(-1));
}

static void next_level()
{
    entity_remove_all();
    reset_ship();
    reset_level();
    game_started = true;
    game_cleared_count = 0;
    game_over_count = 0;
}

static void handle_title()
{
    game_title_count++;
    gfx_print("\n\n\n\n\n\n\n\n               ASTEROIDS");
    if (game_title_count % 30 < 20)
    {
        gfx_print("\n\n\n\n\n\n\n         - PUSH START BUTTON -");
    }
    else {
        gfx_print("\n\n\n\n\n\n\n");
    }
    gfx_print("\n\n\n\n\n\n\n\n           (C)2023 danx2016");

    if (!game_started && input_is_action_just_pressed(0, START))
    {
        start_new_game();
    }
}

static void handle_playing()
{
    // check level cleared (all asteroids was destroyed?)
    if (asteroid_tag_start->next_entity == asteroid_tag_end)
    {
        gfx_print("\n\n\n\n\n\n\n\n\n\n            LEVEL CLEARED!");
        game_cleared_count++;
    }

    if (game_cleared_count >= 360)
    {
        next_level();
    }

    Entity *ship = entity_find_first_type(SHIP);
    if (ship == NULL)
    {
        ship_destroyed_count++;
    }
    else if (ship->is_invincible)
    {
        gfx_print("\n\n\n\n\n\n\n\n\n                READY?");
        go_count = 30;
    }
    else if (!ship->is_invincible && go_count > 0)
    {
        gfx_print("\n\n\n\n\n\n\n\n\n                 GO!");
        go_count--;
    }

    if (ship_destroyed_count >= 180)
    {
        if (lives > 0)
        {
            reset_ship();
            lives--;
        }
        else {
            audio_stop_music();
            gfx_print("\n\n\n\n\n\n\n\n\n\n               GAME OVER");
            game_over_count++;
            if (game_over_count >= 540)
            {
                start_title();
            }
        }
    }
}

static void update_lives(uint8_t *text, size_t index)
{
    text[index] = lives + '0';
}

static void update_score(uint8_t *text, size_t index)
{
    for (int i = index; i > index - 6; i--)
    {
        text[i] = '0';
    }

    uint32_t score_tmp = score;
    while (score_tmp > 0)
    {
        uint8_t dig = score_tmp % 10;
        score_tmp = score_tmp / 10;
        text[index--] = dig + '0';
    }
}

void game_add_score(uint32_t points)
{
    score += points;
}

static void game_init_all()
{
    mem_init();
    gfx_init();
    audio_init();
    math_init();
    input_init();
    entity_init();
    ship_init();
    asteroid_init();
}

void game_start()
{
    game_init_all();
    
    // mod music 
    VSyncCallback(audio_play_next_sample);

    start_title();

    uint8_t *hud = "\n\n\n LIVES: _               SCORE: ______\n";

    while (true)
    {
        input_update();
        entity_update_all();
        entity_render_all();

        // show lives and score
        update_lives(hud, 11);
        update_score(hud, 39);
        
        gfx_print(hud);

        // handle title & playing states
        if (!game_started)
        {
            handle_title();
        }
        else
        {
            handle_playing();
        }

        gfx_swap_buffers();
    }    
}
