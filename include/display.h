//
// Created by Lenovo on 2024/1/9.
//

#ifndef FIN_PROGRAM_DISPLAY_H
#define FIN_PROGRAM_DISPLAY_H

#include <basic.h>
//load text, must be destroyed after loaded
extern TTF_Font *load_text(const char *path, int size);
//load graph, must be destroyed after loaded
extern SDL_Texture *load_graph(const char *path);
//add the graph to the renderer, not displayed
extern void display_graph(SDL_Texture *texture, Coord rect);
//display text
extern void display_text(TTF_Font *font, const char *content, SDL_Color color, Coord rect);
//set background color, not displayed
extern void set_background_color(SDL_Color color);
//Display everything
extern void display();
//Destroy SDL_Texture *texture
extern void destroy_graph(SDL_Texture *texture);
//Destroy TTF_Font *font
extern void destroy_ttf(TTF_Font *font);
//open a file
extern FILE *open_file(const char *path, const char *type);

#endif //FIN_PROGRAM_DISPLAY_H