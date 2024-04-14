#include <main.h>

App app;

int main(int argc , char *argv[]){
    init_all();
    init_keyboard();

    atexit(&quit_all);
    atexit(&quit_keyboard);

    launch();
    do{
        while(1){
            int next_move = do_menu_logic();
            if(next_move == GAME){
                break;
            }
            if(next_move == RECORDS){
                record();
            }
            if(next_move == RULES){
                rules();
            }
        }
    }while(game());

    return 0;
}

//initialize SDL
static void init_all(){
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0){
        HANDLE_ERROR("Init SDL");
    }
    if(TTF_Init() == -1){
        HANDLE_ERROR("Init TTF");
    }
    if(!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)){
        HANDLE_ERROR("Init Image");
    }
    if(!Mix_Init(MIX_INIT_MP3 | MIX_INIT_FLAC)){
        HANDLE_ERROR("Init Mixer");
    }
}

//quit SDL
static void quit_all(){
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
}

static void init_keyboard(){
    app.keyboard = (bool *)calloc(SDL_NUM_SCANCODES,sizeof(bool));
}

static void quit_keyboard(){
    free(app.keyboard);
}
