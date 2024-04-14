//Genshin , Launch !

#ifndef FIN_PROGRAM_LAUNCH_H
#define FIN_PROGRAM_LAUNCH_H

#include <basic.h>
#include <color.h>
#include <display.h>
#include <audio.h>
#include <event.h>

#define NUM_WIDGETS 6

typedef struct {
    const char *text;
    Coord coord;
    void (*action)();
} Widget;

static Widget *widgets;

//Launch the game
extern void launch();
//Create window and renderer
static void init();
//do menu logic
int do_menu_logic();
//paint the menu
static void draw_menu(bool pointer, TTF_Font *font, SDL_Texture *background_texture);
//init *widget
static void init_widget();
//free *widget
static void free_widget();

static void quit_game();

static void load_game();
//显示历史记录
extern void record();
//显示游戏规则
extern void rules();

#endif