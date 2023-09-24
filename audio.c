#include <stdbool.h>

#include "audio.h"
#include "third_party/nugget/modplayer/modplayer.h"

static uint32_t music_time;

static bool is_music_paused = false;
static bool is_music_stopped = false;

void audio_init()
{
    MOD_SetMusicVolume(8000);
    is_music_paused = true;
}

uint32_t next_sample_index;
void audio_play_next_sample()
{
    next_sample_index++;
    if (!is_music_paused && (next_sample_index % music_time) < (music_time - 1))
    {
        MOD_Poll();
    }
}

void audio_stop_music()
{
    if (!is_music_stopped)
    {
        MOD_Silence();
        is_music_stopped = true;
    }
}

void audio_pause_music()
{
    is_music_paused = true;
}

void audio_resume_music()
{
    is_music_paused = false;
}

void audio_play_music(uint8_t music_id)
{
    is_music_paused = false;
    is_music_stopped = false;

    switch (music_id)
    {
    case MUSIC_ID_TITLE:
        MOD_Load((struct MODFileFormat*) _binary_assets_musics_music_hit_start);
        music_time = 5;
        break;

    case MUSIC_ID_PLAYING:
        MOD_Load((struct MODFileFormat*) _binary_assets_musics_music2_hit_start);
        music_time = 10;
        break;
    }
}

void audio_play_sound(uint8_t sound_id)
{
    MOD_PlaySoundEffect(sound_id - 20, sound_id, 128, 63);
}