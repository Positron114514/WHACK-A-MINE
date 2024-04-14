//launch the game

#include <launch.h>


//启动！
extern void launch(){
    //初始化
    init();
    init_mix();
    TTF_Font *title_font = load_text("./res/ttf/GenshinDefault.ttf",72);
    //颜色和位置设置
    Coord title_coord = {320, 300};
    SDL_Color color_title = BLACK;
    color_title.a = 0x00;
    SDL_Color color_background = WHITE;

    set_background_color(color_background);
    display();
    //放音乐！
    Mix_Music *launch_music = load_music("./res/music/Launch_new_version.mp3");
    //设置音量为0
    Mix_VolumeMusic(0);
    //播放音乐
    play_music_fade_in(launch_music, 100, 16);

    //free music

    SDL_Delay(1000);
    //NAME渐入
    while(color_title.a < 255) {
        //显示背景图片
        set_background_color(color_background);
        //显示标题
        display_text(title_font, NAME, color_title, title_coord);
        display();

        color_title.a += 3;
        SDL_Delay(32);
    }
    SDL_Delay(2500);

    //NAME渐出
    while(color_title.a > 5) {
        //显示背景图片
        set_background_color(color_background);
        //显示标题
        display_text(title_font, NAME, color_title, title_coord);
        display();

        color_title.a -= 3;
        SDL_Delay(16);
    }
    while(Mix_PlayingMusic()){
        SDL_Delay(32);
    }


    destroy_ttf(title_font);

    //释放
    destroy_music(launch_music);
}

//初始化window && renderer
static void init(){
    //创建window
    app.window = SDL_CreateWindow(NAME,100,100,WINDOW_WIDTH,WINDOW_HEIGHT,SDL_WINDOW_SHOWN);
    if(app.window == NULL){
        HANDLE_ERROR("Create Window");
    }
    //创建renderer
    app.renderer = SDL_CreateRenderer(app.window , 0 , SDL_RENDERER_ACCELERATED);
    if(app.renderer == NULL){
        HANDLE_ERROR("Create Renderer");
    }
}

//只有第一次启动时调用 launch 函数，其余的返回 menu 操作可以直接调用这个函数
int do_menu_logic(){
    init_mix();
    TTF_Font *menu_font = load_text("./res/ttf/Broken.ttf",48);
    SDL_Texture *background_texture = load_graph("./res/graph/menu_background.png");
    SDL_Event event;

    init_widget();

    widgets[0].coord.x = 540 ,widgets[0].coord.y = 360;
    widgets[1].coord.x = 540 ,widgets[1].coord.y = 360 +LINE_SPACE;

    bool widget_pointer = 0;
    draw_menu(widget_pointer, menu_font, background_texture);
    //加载音效
    Mix_Chunk* sound = load_fx("./res/music/sine_pluck.mp3");
    log("Load menu successfully.\n");

    clean_rubbish_events();
    while(SDL_WaitEvent(&event)){
        int ret = 0;

        //对按下状态做修改
        do_event(event);
        // 在start和quit中切换
        if(app.keyboard[SDL_SCANCODE_UP] ||app.keyboard[SDL_SCANCODE_RIGHT] || app.keyboard[SDL_SCANCODE_DOWN] || app.keyboard[SDL_SCANCODE_LEFT]){
            widget_pointer = !widget_pointer;
            //其实是从audio.h的play_fx函数里摘出来的，直接play会导致pointer位置改变有延迟，所以play和这个放一起
            //这么想是不是play_music和play_fx都不能单独放在函数里啊，因为没法在播放完之前free，一直检测还会导致不能边放音乐边进行其他操作
            //也不清楚Mix_LoadWAV具体是怎么实现的
            //2024/1/11 更新了一下函数实现，确实能写出来其实
            play_fx(sound,1);

            draw_menu(widget_pointer, menu_font, background_texture);

        }
        //返回结算的情况
        //退出
        if(app.keyboard[SDL_SCANCODE_SPACE] || app.keyboard[SDL_SCANCODE_RETURN]){
            ret = 1;
        }
        //进入游戏or退出
        if(app.left_mouse){
            if(event.button.x > 538 && event.button.x < 698 && event.button.y >364 && event.button.y < 408){
                widget_pointer = 0;
                ret = 1;
            }else if(event.button.x > 534 && event.button.x < 687 && event.button.y >428 && event.button.y < 477){
                widget_pointer = 1;
                ret = 1;
            }
            if(event.button.x > 40 && event.button.x < 263){
                if(event.button.y >500 && event.button.y < 550){
                    ret = 2;
                }else if(event.button.y >500 + LINE_SPACE && event.button.y < 550 + LINE_SPACE){
                    ret = 3;
                }
            }
        }
        if(app.keyboard[SDL_SCANCODE_ESCAPE]){
            log("Exited by ESC.\n");
            exit(EXIT_SUCCESS);
        }
        //

        if(ret) {
            if(widget_pointer == 1){
                log("Exited by quit game.\n");
                exit(EXIT_SUCCESS);
            }
            destroy_fx(sound);
            destroy_ttf(menu_font);
            free_widget();
            quit_mix();
            return ret - 1;
        }

        SDL_Delay(30);
    }
    quit_mix();
}


//画菜单
static void draw_menu(bool pointer, TTF_Font *font, SDL_Texture *background_texture){
//    SDL_Color color_background = WHITE;
//    set_background_color(color_background);
    //显示背景
    Coord null = {-1, -1};
    display_graph(background_texture, null);
    //显示文字
    Coord widget_coord = widgets[0].coord;
    display_text(font, widgets[0].text, (SDL_Color)BLACK, widget_coord);
    widget_coord.y += LINE_SPACE;
    display_text(font, widgets[1].text, (SDL_Color)BLACK, widget_coord);

    Coord pointer_coord = {widgets[pointer].coord.x - 48, widgets[pointer].coord.y};
    display_text(font, ">", (SDL_Color)BLACK, pointer_coord);

    log("> %s\n",widgets[pointer].text);
    // 2024/1/16 新增检查记录功能和查看规则功能
    Coord coord_records = {40, 500};
    display_text(font, "records", (SDL_Color) BLACK, coord_records);
    display_text(font, "rules", (SDL_Color) BLACK, (Coord){40, 500 + LINE_SPACE});

    display();
}

//init *widget
static void init_widget(){
    widgets = (Widget *)calloc(NUM_WIDGETS,sizeof(Widget));
    widgets[0] = (Widget) {"START", 0, 0, load_game};
    widgets[1] = (Widget) {"QUIT", 0, 0,quit_game};
}
//free *widget
static void free_widget(){
    free(widgets);
}

static void act_widget(int selection){
    void(*action)() = widgets[selection].action;
    if(action){
        action();
    }
}

static void quit_game(){
    log("Exited by quit_game.\n");
    exit(0);
}

static void load_game(){
    log("Load Game.\n");
//    fflush(stdout);
}

extern void record(){
    FILE *records = open_file("./res/file/records.txt", "r+");
    char str[20][80] = {0};
    char ch;
    int num = 0;
    for(int  i = 0; i < 20; i++){
        if(fscanf(records, "%[^\n]s%c",str[i], &ch) == -1){
            break;
        }
        if(ch != '\n'){
            ch = fscanf(records,"%c",&ch);
        }
        num++;
    }

    TTF_Font *record_font = load_text("./res/ttf/Broken.ttf",24);
    TTF_Font *back_to_menu_font = load_text("./res/ttf/Broken.ttf",36);
    Coord records_coord = {40, 40};

    SDL_RenderClear(app.renderer);
    set_background_color((SDL_Color) WHITE);
    for(int i = 0; i < num / 2; i++){
        strcat(str[2 * i], " ");
        strcat(str[2 * i],str[2 * i + 1]);
        display_text(record_font, str[2 * i], (SDL_Color) DARK_BLUE, records_coord);
        records_coord.y += LINE_SPACE;
//        printf("OK in line %d.\n",i);
    }
    records_coord.x = 800;
    display_text(back_to_menu_font, "BACK TO MENU", (SDL_Color) BLACK, records_coord);

    display();

    SDL_Event event;
    bool ret = false;
    clean_rubbish_events();
    while(SDL_WaitEvent(&event)){
        do_event(event);

        if(app.keyboard[SDL_SCANCODE_SPACE] || app.keyboard[SDL_SCANCODE_ESCAPE]){
            ret = true;
        }

        if(app.left_mouse){
            if(event.button.y >= records_coord.y && event.button.y <= records_coord.y + 48
            && records_coord.x >= 800 && records_coord.x <= 1200){
                ret = true;
            }
        }

        if(ret){
            fclose(records);
            destroy_ttf(back_to_menu_font);
            destroy_ttf(record_font);
            return;
        }
    }
}

extern void rules(){
    SDL_RenderClear(app.renderer);
    FILE *story = open_file("./res/file/rules", "r");
    TTF_Font *text_font = load_text("./res/ttf/Broken.ttf", 16);
    TTF_Font *hint = load_text("./res/ttf/Broken.ttf", 24);
    Mix_Chunk *turn_pages = load_fx("./res/music/book.mp3");
    int line_space = 32;
    Coord coord = {40, 40};
    char story_text[250] = {0};
    int num = 0;
    while(fgets(story_text, sizeof(story_text), story) != NULL){
//        printf("story_text[%d] = %s\n", num, story_text);
        display_text(text_font, story_text, (SDL_Color) BLACK, coord);
//        printf("OK loading text[%d].\n", num);
        coord.y += line_space;
        num++;
    }

    display();
    unsigned int time_start = SDL_GetTicks();
    SDL_Event event;
    clean_rubbish_events();
    bool displayed = false;
    while(1){
        SDL_PollEvent(&event);
        do_event(event);

        //检测是否可以翻页
        if(!displayed){
            if(SDL_GetTicks() - time_start > 2500){
                displayed = true;
                display_text(hint, "NEXT PAGE >", (SDL_Color) DARK_PURPLE, (Coord) {1000, coord.y});
                display();
            }
        }else {
            if(app.keyboard[SDL_SCANCODE_SPACE] || app.keyboard[SDL_SCANCODE_DOWN] || app.keyboard[SDL_SCANCODE_RIGHT]) {
                break;
            }
            if(app.left_mouse){
                if(event.button.x > 960 && event.button.x < 1200 && event.button.y > coord.y && event.button.y < coord.y + LINE_SPACE){
                    break;
                }
            }
        }

        if(app.keyboard[SDL_SCANCODE_ESCAPE]){
            fclose(story);
            destroy_ttf(hint);
            destroy_ttf(text_font);
            destroy_fx(turn_pages);
            return;
        }

        SDL_Delay(32);
    }

    coord = (Coord) {40, 40};
    SDL_RenderClear(app.renderer);

    fclose(story);
    // 播放翻书声
    play_fx(turn_pages, -1);

    FILE *rules = open_file("./res/file/rules_pt2", "r");
    while(fgets(story_text, sizeof(story_text), rules) != NULL){
        display_text(text_font, story_text, (SDL_Color) BLACK, coord);
        coord.y += line_space;
    }
    display();

    time_start = SDL_GetTicks();
    clean_rubbish_events();
    displayed = false;
    while(1){
        SDL_PollEvent(&event);
        do_event(event);

        if(!displayed){
            if(SDL_GetTicks() - time_start > 2500){
                displayed = true;
                display_text(hint, "BACK TO MENU", (SDL_Color) DARK_PURPLE, (Coord) {960, coord.y});
                display();
            }
        }else {
            if (app.keyboard[SDL_SCANCODE_SPACE] || app.keyboard[SDL_SCANCODE_DOWN] ||
                app.keyboard[SDL_SCANCODE_RIGHT]) {
                break;
            }
            if(app.left_mouse){
                if(event.button.x > 960 && event.button.x < 1200 && event.button.y > coord.y && event.button.y < coord.y + LINE_SPACE){
                    break;
                }
            }
        }

        if(app.keyboard[SDL_SCANCODE_ESCAPE]){
            break;
        }

        SDL_Delay(32);
    }
    fclose(rules);
    destroy_ttf(hint);
    destroy_ttf(text_font);
    destroy_fx(turn_pages);
}

// 2024/1/10
// 终于把launch写完了啊哈哈哈，写了两天，边学边写，遇到不会的就问GPT，GPT回答看不懂的就回去翻文档。助教的视频也已经看了114514遍了。虽然但是写的还是一坨大便
// 史

// 2024/1/11
// reconstruct成功！这次把display和audio一系列函数的实现改了一下，于是又把launch.c重构了一遍，竟然没出bug，好开心！！！

// 2024/1/16
// 感觉这个文件明显没有game.c写的好
// 增加了翻阅历史记录功能以及规则介绍功能
// 2024/1/18该功能完工