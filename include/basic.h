//包含一些基本的宏定义及头文件

#ifndef FIN_PROGRAM_BASIC_H
#define FIN_PROGRAM_BASIC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <time.h>
#include <windows.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_main.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"

#undef main

//#define __MACRO__

#ifdef __MACRO__
#define log(...) printf(__VA_ARGS__)
#else
#define log(...)
#endif

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 760

#define HANDLE_ERROR(msg) log(msg ": %s\n",SDL_GetError()); \
exit(EXIT_FAILURE)

#define Max(a, b) (a) > (b) ? (a) : (b)
#define Min(a, b) (a) > (b) ? (b) : (a)

#define NAME "WHACK-A-MINE"

#define LINE_SPACE 64

#define GAME 0
#define RECORDS 1
#define RULES 2



typedef struct{
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool *keyboard;
    bool left_mouse;
    bool right_mouse;
}  App;

typedef struct{
    int x;
    int y;
} Coord;

extern App app;

#endif