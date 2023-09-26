#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <stdint.h>

#define SOUND_START_CHANNEL 5

#define SOUND_ID_START      27
#define SOUND_ID_SHOOT      28
#define SOUND_ID_EXPLOSION  29

#define MUSIC_ID_TITLE     1
#define MUSIC_ID_PLAYING   2

// mod title music data
extern const uint8_t _binary_assets_musics_music_hit_start[];
extern const uint8_t _binary_assets_musics_music_hit_end[];
extern const uint8_t _binary_assets_musics_music_hit_size[];

// mod playing music data
extern const uint8_t _binary_assets_musics_music2_hit_start[];
extern const uint8_t _binary_assets_musics_music2_hit_end[];
extern const uint8_t _binary_assets_musics_music2_hit_size[];


extern void audio_init();
extern void audio_play_music(uint8_t music_id);
extern void audio_stop_music();
extern void audio_pause_music();
extern void audio_resume_music();

extern void audio_play_sound(uint8_t sound_id);

#endif /* _AUDIO_H_ */