//
// Created by Lenovo on 2024/1/10.
//

#include <event.h>

extern void handle_events(){

}

extern void clean_rubbish_events(){
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
    memset(app.keyboard, 0, SDL_NUM_SCANCODES * sizeof(bool));
    app.left_mouse = 0;
    app.right_mouse = 0;
}


extern bool do_event(SDL_Event event){
    switch (event.type) {
        case SDL_QUIT :
            log("Exited by 'x'\n");
            exit(EXIT_SUCCESS);
        case SDL_MOUSEBUTTONUP:
            log("Mouse button up at (%d, %d)\n",event.button.x,event.button.y);
            if(event.button.button == SDL_BUTTON_LEFT) {
                app.left_mouse = false;
            }
            if(event.button.button == SDL_BUTTON_RIGHT) {
                app.right_mouse = false;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            log("Mouse button down at (%d, %d)\n",event.button.x,event.button.y);
            if(event.button.button == SDL_BUTTON_LEFT) {
                app.left_mouse = true;
            }
            if(event.button.button == SDL_BUTTON_RIGHT) {
                app.right_mouse = true;
            }
            break;
        case SDL_KEYUP:
            log("Key up at %s\n", SDL_GetKeyName(event.key.keysym.sym));
            app.keyboard[event.key.keysym.scancode] = false;
            break;
        case SDL_KEYDOWN:
            log("Key down at %s\n", SDL_GetKeyName(event.key.keysym.sym));
            app.keyboard[event.key.keysym.scancode] = true;
            break;
        case SDL_MOUSEMOTION:
            return 0;
        default:
            break;
    }
//    fflush(stdout);
    return 1;
}