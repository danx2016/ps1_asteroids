#include <stdbool.h>
#include <stdio.h>
#include <libapi.h>

#include "audio.h"
#include "third_party/nugget/modplayer/modplayer.h"

static bool is_music_paused;
static bool is_music_stopped;

static uint32_t bmp_time;

#define BPM_TIME_CONV 140
#define BPM_TIME_INC  15000

// invoked every ~0.015 secs
int32_t audio_play_next_sample()
{
    if (is_music_paused || is_music_stopped)
    {
        return 1;
    }

    bmp_time += BPM_TIME_INC;
    while (bmp_time >= MOD_BPM * BPM_TIME_CONV)
    {
        bmp_time -= MOD_BPM * BPM_TIME_CONV;
        MOD_Poll();
    }
    
    return 1;
}

void audio_init()
{
    is_music_paused = true;
    is_music_stopped = true;

    // setup the root counter 2 interruption handler to play 
    // mod music at correct timing regardless of NTSC/PAL video mode
    EnterCriticalSection();
    uint32_t ev = OpenEvent(RCntCNT2, EvSpINT, EvMdINTR, audio_play_next_sample);
    EnableEvent(ev);
    SetRCnt(RCntCNT2, 0xffff, RCntMdINTR);
    ExitCriticalSection();
    StartRCnt(RCntCNT2);

    MOD_SetMusicVolume(8000);
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
            break;

        case MUSIC_ID_PLAYING:
            MOD_Load((struct MODFileFormat*) _binary_assets_musics_music2_hit_start);
            break;
    }
}

void audio_play_sound(uint8_t sound_id)
{
    #define SPU_CHANNEL_START_FIX 20
    MOD_PlaySoundEffect(sound_id - SPU_CHANNEL_START_FIX, sound_id, 128, 63);
}