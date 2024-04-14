//
// Created by Lenovo on 2024/1/11.
//

#ifndef FIN_PROGRAM_GAME_H
#define FIN_PROGRAM_GAME_H

#include <basic.h>
#include <display.h>
#include <audio.h>
#include <event.h>
#include <color.h>

#define BLOCK_SIZE 62

#define MAP_SIZE 10

#define BLOCK_SPACE 8

#define MAP_Y ((WINDOW_HEIGHT - (BLOCK_SIZE + BLOCK_SPACE) * 10)/2)
#define MAP_X (WINDOW_WIDTH - ((BLOCK_SIZE + BLOCK_SPACE) * 10 + MAP_Y))

#define NUM_HEART 10
#define MINE_NUM 10
#define BGM_NUM 9

#define SHREW_EXIST_TIME 2000
#define SHREW_SPAWN_TIME 3000
#define MARK_ADD 260
#define FASTER 0.85

#define SHREW_TYPE 3
#define SHREW_1_POSSIBILITY 10
#define SHREW_2_POSSIBILITY 2
#define SHREW_3_POSSIBILITY 1
#define TOTAL_POSSIBILITY (SHREW_1_POSSIBILITY + SHREW_2_POSSIBILITY + SHREW_3_POSSIBILITY)

//shrew type 0:none  1:normal  2:bomb  3:respawn
typedef struct {
    int type;
    unsigned int spawn_time;
} Shrew;

typedef struct{
    bool shown;
    bool mine;
    bool mine_found;
    bool show_mine_around;
    Shrew shrew;
    int mine_around;
    Coord coord;
} Block;

typedef struct{
    const char *name;
    Mix_Music *music;
} Bgm;

typedef struct{
    SDL_Texture *background_texture;
    SDL_Texture *block_texture;
    SDL_Texture *shrew_graph[3];
    SDL_Texture *heart_texture;
    SDL_Texture *mine_texture;
    SDL_Texture *empty_texture;
    SDL_Texture *found_texture;
    //字体
    TTF_Font *text_font;
    TTF_Font *bgm_font;
    //音乐
    Bgm bgm[9];
    //音效
    Mix_Chunk *fx_hit_shrew;  // channel: 1
    Mix_Chunk *fx_shrew_laugh; // channel: 2
    Mix_Chunk *fx_hurt;               //channel: 3
    Mix_Chunk *fx_explosion;  //channel: 4
    Mix_Chunk *fx_found_mine; //channel: 5
} Resource;

typedef struct{
    unsigned int last_spawn_time;
    unsigned int launch_time;
    int minute;
    int second;
} Time ;

typedef struct{
    Resource res;
    int num_heart;
    int mine_found;
    int rand_bgm;
    int shrew_hit;
    Time time;
} Game;

typedef struct{
    char win_or_lose;
    int mine_found;
    int heart_left;
    int minute;
    int second;
} Record ;

static Game app_game;
static Block map[MAP_SIZE][MAP_SIZE];

//运行游戏
extern bool game();
//init the map
//初始化游戏
static void load_game();

static void paint_game();

static void init_map();
//paint map (blocks)
static void paint_map();
//paint heart
static void paint_heart();
//paint HEALTH and MINE FOUND , MINE LEFT
static void paint_texts();
//paint bgm name
static void paint_bgm();
//paint "Press ESC to go back to the menu."
static void paint_hint();
//棋盘随机布雷
static void rand_map();
//game over
static void dead();
//respawn mine
static void respawn_mine(int x, int y);
//spawn 1 shrew
static void spawn_shrew();
//hit a shrew
static void hit_a_shrew(int x, int y);
//decide the color of the number based on the num of the mine around
static SDL_Color decide_color(int num);
//find a mine
static void find_a_mine(int x, int y);
//count the num of the mine around the block
static void count_around(int x, int y);
//game over
static void do_exit_logic();
//record game
static void record_game();
//quit_game
static void quit_game();
//检验是否更换bgm
static void play_bgm();
//paint shrew and exam if they exist anymore
static void paint_shrew();
//paint time
static void paint_time();
//exam if this is the best record
static bool exam_best();
//达到最佳记录时的界面
static void best();
//显示一共打了多少只地鼠
static void paint_shrew_hit();
//游戏结束
static void game_end(bool win);
//show the map
static void show_map();


#endif //FIN_PROGRAM_GAME_H