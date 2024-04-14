//
// Created by Lenovo on 2024/1/10.
//
//处理事件
#ifndef FIN_PROGRAM_EVENT_H
#define FIN_PROGRAM_EVENT_H

#include <basic.h>

#define MAX_EVENT_NUMBER 100

static void clean_events(Uint32 event_type);

extern void clean_rubbish_events();

extern bool do_event(SDL_Event event);

#endif //FIN_PROGRAM_EVENT_H