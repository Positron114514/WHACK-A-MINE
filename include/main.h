//
// Created by Lenovo on 2024/1/9.
//

#ifndef FIN_PROGRAM_MAIN_H
#define FIN_PROGRAM_MAIN_H

#include <basic.h>
#include <color.h>
#include <launch.h>
#include <audio.h>
#include <display.h>
#include <event.h>
#include <game.h>

//initialize SDL
static void init_all();

//quit SDL
static void quit_all();

//init app.keyboard
static void init_keyboard();

//quit app.keyboard
static void quit_keyboard();

//static void init_mouse();
//
//static void quit_mouse();


#endif //FIN_PROGRAM_MAIN_H