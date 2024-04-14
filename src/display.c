//
// Created by Lenovo on 2024/1/9.
//

//单独写了display函数便于解决闪烁问题
#include <display.h>

extern TTF_Font *load_text(const char *path, int size){
    TTF_Font *font = TTF_OpenFont(path,size);
    if(font == NULL){
        HANDLE_ERROR("Load Font");
    }
    return font;
}

extern SDL_Texture *load_graph(const char *path){
    SDL_Surface *surface = IMG_Load(path);
    if(surface == NULL){
        HANDLE_ERROR("Load Image Surface");
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(app.renderer,surface);
    if(surface == NULL){
        HANDLE_ERROR("Load Image Surface");
    }
    SDL_FreeSurface(surface);
    return texture;
}

extern void destroy_graph(SDL_Texture *texture){
    SDL_DestroyTexture(texture);
}

extern void destroy_ttf(TTF_Font *font){
    TTF_CloseFont(font);
}

//add the text to the renderer, not displayed
extern void display_text(TTF_Font *font, const char *content, SDL_Color color, Coord rect){
    //创建surface和texture变量
    SDL_Surface *surface = TTF_RenderText_Blended(font, content, color);
    if(surface == NULL){
        HANDLE_ERROR("Load Text Surface");
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(app.renderer, surface);
    if(texture == NULL){
        HANDLE_ERROR("Load Text Texture");
    }
    //用于形确定文字放置位置
    SDL_Rect rect_title = {.x = rect.x, .y = rect.y};
    SDL_QueryTexture(texture, NULL, NULL, &rect_title.w, &rect_title.h);
    //加载到渲染器并显示
    SDL_RenderCopy(app.renderer, texture, NULL, &rect_title);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

//add the graph to the renderer, not displayed
extern void display_graph(SDL_Texture *texture, Coord rect){
    //载入图片surface & texture
    if(rect.x == -1){
        SDL_RenderCopy(app.renderer, texture, NULL, NULL);
    }
    else {
        SDL_Rect rect_start = {.x = rect.x, .y = rect.y};
        SDL_QueryTexture(texture, NULL, NULL, &rect_start.w, &rect_start.h);
        //加载到renderer
        SDL_RenderCopy(app.renderer, texture, NULL, &rect_start);
    }
}

//set background color, not displayed
extern void set_background_color(SDL_Color color){
    SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(app.renderer);
}

//Display everything
extern void display(){
    SDL_RenderPresent(app.renderer);
}

extern FILE *open_file(const char *path, const char *type){
    FILE *file = fopen(path, type);
    if(file == NULL){
        HANDLE_ERROR("Open file");
    }
    return file;
}