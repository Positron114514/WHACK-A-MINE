//
// Created by Lenovo on 2024/1/9.
//

#ifndef FIN_PROGRAM_AUDIO_H
#define FIN_PROGRAM_AUDIO_H

#include <basic.h>

//Init the audio. You should use function "quit_music" to release the memory
extern void init_mix();
//load music. must free after used
extern Mix_Music *load_music(const char *path);
// play music
// You must use init_mix before using this function.
extern void play_music(Mix_Music *music);
//play music fade in
extern void play_music_fade_in(Mix_Music *music, int fade_in_step, int fade_in_delay);
//free Mix_music*
extern void destroy_music(Mix_Music *music);
//load fx. must free after used
extern Mix_Chunk *load_fx(const char *path);
// play fx
// You must use init_mix before using this function.
extern void play_fx(Mix_Chunk *sound, int channel);
//free Mix_Chunk*
extern void destroy_fx(Mix_Chunk *sound);

//Quit the audio
extern void quit_mix();

#endif //FIN_PROGRAM_AUDIO_H
