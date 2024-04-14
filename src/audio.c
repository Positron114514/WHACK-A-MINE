//
// Created by Lenovo on 2024/1/9.
//
#include <audio.h>
//Init the audio. You should use function "quit_music" to release the memory
extern void init_mix(){
    //初始化打开参数
    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,2048)){
        HANDLE_ERROR("Open Audio");
    }
}
//Quit the audio
extern void quit_mix(){
    Mix_CloseAudio();
}

extern Mix_Music *load_music(const char *path){
    Mix_Music *music = Mix_LoadMUS(path);
    if (music == NULL) {
        HANDLE_ERROR("Load music");
    }
    return music;
}

// play music
// You must use init_mix before using this function.
extern void play_music(Mix_Music *music){
    if(Mix_PlayMusic(music,0) == -1){
        HANDLE_ERROR("Play music");
    }
}

extern void play_music_fade_in(Mix_Music * music, int fade_in_step, int fade_in_delay){
    play_music(music);
    for (int i = 0; i <= fade_in_step; i++) {
        int volume = i * MIX_MAX_VOLUME / fade_in_step;
        Mix_VolumeMusic(volume);
        // 添加一点延时
        SDL_Delay(fade_in_delay);
    }
}

extern void destroy_music(Mix_Music *music){
    Mix_FreeMusic(music);
}

extern Mix_Chunk *load_fx(const char *path){
    Mix_Chunk* sound = Mix_LoadWAV(path);
    if (sound == NULL) {
        HANDLE_ERROR("Load FX");
    }
    return sound;
}

// play fx
// You must use init_mix before using this function.
extern void play_fx(Mix_Chunk *sound, int channel){
    if(Mix_PlayChannel(channel, sound, 0) == -1){
        HANDLE_ERROR("Play fx");
    }
}

extern void destroy_fx(Mix_Chunk *sound){
    Mix_FreeChunk(sound);
}
