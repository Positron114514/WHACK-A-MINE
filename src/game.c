//
// Created by Lenovo on 2024/1/11.
//
#include <game.h>
//游戏主体

extern bool game(){
    //init && load
    init_mix();
    load_game();
//    Coord null_coord = {-1, -1};
    init_map();
    paint_game();
    //随机化棋盘界面
    rand_map();
    log("Load map successfully.\n");

    SDL_Event event;
    //游戏运行
    clean_rubbish_events();
    while(1){
        //判断是否游戏结束
        if(app_game.num_heart <= 0|| app.keyboard[SDL_SCANCODE_ESCAPE] || app_game.mine_found == MINE_NUM){
            do_exit_logic();
            return 1;
        }

        SDL_PollEvent(&event);
        if(do_event(event)) {
            //左键 是不是地鼠
            if (app.left_mouse) {
                // 2024/1/14增加两种新地鼠：混乱地鼠 (type = 2) ,被击中后随机分配周围地雷, 不打扣一滴血; 核弹地鼠(type = 3), 击中后若其所在格子是地雷, 则地雷爆炸, 玩家扣5滴血
                if (event.button.x - MAP_X >= 0 && event.button.y - MAP_Y >= 0) {
                    int x = (event.button.x - MAP_X) / (BLOCK_SPACE + BLOCK_SIZE);
                    int y = (event.button.y - MAP_Y) / (BLOCK_SPACE + BLOCK_SIZE);
                    if (x >= 0 && x < 10 && y >= 0 && y < 10) {
                        if (map[x][y].shrew.type) {
                            log("Hit a shrew at (%d, %d).\n", x, y);
                            play_fx(app_game.res.fx_hit_shrew, 1);
                            hit_a_shrew(x, y);
                        } else {
                            log("Hit wrong place at (%d, %d).\n", x, y);
                            app_game.num_heart--;
                            play_fx(app_game.res.fx_hurt, 3);
                            respawn_mine(x, y);
                        }
                    }
                }
            }
            //右键 是不是地雷
            if (app.right_mouse) {
                if (event.button.x - MAP_X >= 0 && event.button.y - MAP_Y >= 0) {
                    int x = (event.button.x - MAP_X) / (BLOCK_SPACE + BLOCK_SIZE);
                    int y = (event.button.y - MAP_Y) / (BLOCK_SPACE + BLOCK_SIZE);
                    if (x >= 0 && x < 10 && y >= 0 && y < 10) {
                        //是地雷
                        if (map[x][y].mine) {
                            log("Mine found at (%d, %d)\n", x, y);
                            find_a_mine(x, y);
                            //mine贴图改一下
                            play_fx(app_game.res.fx_found_mine, 5);
                        } else {    //不是地雷
                            log("Exam wrong place at (%d, %d).\n", x, y);
                            app_game.num_heart--;
                            play_fx(app_game.res.fx_hurt, 3);
                        }

                    }
                }
            }

        }
        //检测换不换bgm
        play_bgm();
        spawn_shrew();
        paint_game();
        //没搞懂为什么delay的值调高就会出现同时hit_the_shrew和hit_wrong_place的bug
        //可能因为帧数太高扫了两次？不应该啊()
        //还是存在，调低帧率也没用
        // 2024/1/13 知道为啥了，鼠标移动也算事件，如果button_down和button_up的坐标不一样就会触发这个bug.
        // 给do_event函数添加了返回值, 如果event类型是mouseMotion就返回0, 其余情况返回1, 可以解决bug.
        SDL_Delay(16);
    }
}

static void load_game(){
    log("Loading....\n");
    //图片
//    app_game.res.background_texture = load_graph("./res/graph/background.png");
    app_game.res.block_texture = load_graph("./res/graph/block.png");
    app_game.res.shrew_graph[0] = load_graph("./res/graph/shrewNormal.png");
    app_game.res.shrew_graph[1] = load_graph("./res/graph/shrewMulti.png");
    app_game.res.shrew_graph[2] = load_graph("./res/graph/shrewChaos.png");
    app_game.res.heart_texture = load_graph("./res/graph/heart.jpg");
    app_game.res.mine_texture = load_graph("./res/graph/mine.png");
    app_game.res.empty_texture = load_graph("./res/graph/empty.jpg");
    app_game.res.found_texture = load_graph("./res/graph/found.png");
    //字体
    app_game.res.text_font = load_text("./res/ttf/Broken.ttf",36);
    app_game.res.bgm_font = load_text("./res/ttf/Broken.ttf",24);
    //音乐
    app_game.res.bgm[0].name = "Meow!!!", app_game.res.bgm[0].music = load_music("./res/music/bgm/Meow!!!.mp3");
    app_game.res.bgm[1].name = "Neon Meow's Jazz Club[Bonus]", app_game.res.bgm[1].music = load_music("./res/music/bgm/NeonMeow'sJazzClub[Bonus].mp3");
    app_game.res.bgm[2].name = "Neverland", app_game.res.bgm[2].music = load_music("./res/music/bgm/Neverland.mp3");
    app_game.res.bgm[3].name =  "Reverie", app_game.res.bgm[3].music = load_music("./res/music/bgm/Reverie.mp3");
    app_game.res.bgm[4].name =  "Neon Meow's Jazz Club Part2", app_game.res.bgm[4].music = load_music("./res/music/bgm/NeonMeow'sJazzClubPart2.mp3");
    app_game.res.bgm[5].name =  "Sweet Time", app_game.res.bgm[5].music = load_music("./res/music/bgm/SweetTime.mp3");
    app_game.res.bgm[6].name =  "Tidechase", app_game.res.bgm[6].music = load_music("./res/music/bgm/Tidechase.mp3");
    app_game.res.bgm[7].name =  "Unfold", app_game.res.bgm[7].music = load_music("./res/music/bgm/Unfold.mp3");
    app_game.res.bgm[8].name =  "Virtual Sickness", app_game.res.bgm[8].music = load_music("./res/music/bgm/VirtualSickness.mp3");
    //音效
    app_game.res.fx_hit_shrew = load_fx("./res/music/hit_shrew.mp3");  // channel: 1
    app_game.res.fx_shrew_laugh = load_fx("./res/music/shrew_spawn.mp3"); // channel: 2
    app_game.res.fx_hurt = load_fx("res/music/hurt.mp3");               //channel: 3
    app_game.res.fx_explosion = load_fx("./res/music/explosion.mp3");  //channel: 4
    app_game.res.fx_found_mine = load_fx("res/music/mark_add.mp3");    //channel: 5
    //数值
    app_game.num_heart = NUM_HEART;
    app_game.mine_found = 0;
    app_game.shrew_hit = 0;
    //时间
    app_game.time.minute = 0;
    app_game.time.second = 0;
    app_game.time.last_spawn_time = SDL_GetTicks();
    app_game.time.launch_time = SDL_GetTicks();

    app_game.rand_bgm = -1;
    log("Load Success.\n");
}

static void init_map(){
    for(int i = 0; i < MAP_SIZE; i++){
        for(int j = 0; j < MAP_SIZE; j++){
            map[i][j].shown = false;
            map[i][j].mine = false;
            map[i][j].shrew.type = 0;
            map[i][j].shrew.spawn_time = 0;
            map[i][j].mine_found = false;
            map[i][j].mine_around = 0;
            map[i][j].show_mine_around = false;
            map[i][j].coord.x = MAP_X + (BLOCK_SPACE + BLOCK_SIZE) * i;
            map[i][j].coord.y = MAP_Y + (BLOCK_SPACE + BLOCK_SIZE) * j;
        }
    }
}

static void paint_game(){
    //更新
    SDL_RenderClear(app.renderer);
    //加载画面
//    display_graph(background_texture,null);
    //显示时间
    paint_time();
    paint_map();
    paint_heart();
    paint_texts();
    paint_hint();
    paint_shrew_hit();
    paint_bgm();
    paint_shrew();
    display();
}

static void paint_time(){
    //calculate time now
    unsigned int current_time = SDL_GetTicks();
    app_game.time.second = (current_time - app_game.time.launch_time) / 1000;
    app_game.time.minute = app_game.time.second / 60;
    app_game.time.second %= 60;
    //paint it
    Coord coord = {40, 430};
    char time[30] = "\0";
    if(((current_time - app_game.time.launch_time) / 500) % 2 == 0) {
        sprintf(time, "Time:  %02d : %02d", app_game.time.minute, app_game.time.second);
    }else{
        sprintf(time, "Time:  %02d   %02d", app_game.time.minute, app_game.time.second);
    }
    display_text(app_game.res.bgm_font, time, (SDL_Color)BLACK, coord);
}

static void paint_map(){
    for(int i = 0; i < MAP_SIZE; i++){
        for(int j = 0; j < MAP_SIZE; j++){
            //检测方块是否暴露
            if(map[i][j].shown == false) {  //没暴露
                display_graph(app_game.res.block_texture, map[i][j].coord);
                if(map[i][j].show_mine_around == true){  //检测是否显示周围有几个雷
                    char mine_around[2] = {map[i][j].mine_around + '0', '\0'};
                    Coord coord_text = {map[i][j].coord.x + 20, map[i][j].coord.y + 15};
                    display_text(app_game.res.text_font, mine_around, decide_color(map[i][j].mine_around), coord_text);

                }
            }else{
                //显示石头
                display_graph(app_game.res.empty_texture, map[i][j].coord);
                if(map[i][j].mine == true){
                    //选择显示红旗还是雷
                    if(map[i][j].mine_found == true){
                        display_graph(app_game.res.found_texture, map[i][j].coord);
                    }else {
                        display_graph(app_game.res.mine_texture, map[i][j].coord);
                    }
                }
            }
        }
    }
}
//paint heart
static void paint_heart(){
    Coord coord = {40, 100};
    int size_heart = 42;
    for(int i = 0; i < app_game.num_heart; i++){
        display_graph(app_game.res.heart_texture, coord);
        coord.x += size_heart;
    }
}
//paint HEALTH and MINE FOUND , MINE LEFT
static void paint_texts(){
    Coord coord_health = {40, 40};

    display_text(app_game.res.text_font, "HEALTH", (SDL_Color)BLACK, coord_health);

    char text_found[16] = "";
    char text_left[16] = "" ;
    sprintf(text_found, "MINE FOUND: %d", app_game.mine_found);
    sprintf(text_left,  "MINE LEFT: %2d", MINE_NUM - app_game.mine_found);

    Coord coord_marks = {40, 200};

    display_text(app_game.res.text_font, text_found, (SDL_Color)BLACK, coord_marks);
    coord_marks.y += LINE_SPACE;
    display_text(app_game.res.text_font, text_left, (SDL_Color)BLACK, coord_marks);
}
//paint bgm name
static void paint_bgm(){
    Coord coord = {40, 660};

    char name[30] = "BGM: ";
    strcat(name, app_game.res.bgm[app_game.rand_bgm].name);

    display_text(app_game.res.bgm_font, name, (SDL_Color)BLACK, coord);
}

static void paint_hint(){
    Coord coord = {40, 350};
    display_text(app_game.res.bgm_font, "Press ESC to escape", (SDL_Color)BLACK, coord);
}

static void paint_shrew_hit(){
    char num_shrew_hit[20] = "";
    sprintf(num_shrew_hit, "Shrew Hit: %d", app_game.shrew_hit);
    Coord coord_shrew_hit = {40, 500};
    display_text(app_game.res.bgm_font, num_shrew_hit, (SDL_Color)BLACK, coord_shrew_hit);
}

static void paint_shrew(){
    //检测地鼠时间
    unsigned int time = SDL_GetTicks();
    for(int i = 0; i < 10 ; i++){
        for(int j = 0; j < 10; j++){
            if(map[i][j].shrew.type){
                if(time - map[i][j].shrew.spawn_time >= SHREW_EXIST_TIME) {
                    //shrew disappear
                    log("Shrew spawned at %d not hit.\n",map[i][j].shrew.spawn_time);
                    app_game.num_heart--;
                    play_fx(app_game.res.fx_hurt, 3);
                    map[i][j].shrew.type = 0;
                }else{
                    //paint shrew
                    display_graph(app_game.res.shrew_graph[map[i][j].shrew.type - 1], map[i][j].coord);
                }
            }
        }
    }
}

static void rand_map(){
    //rand mine
    for(int i = 0; i < MINE_NUM ; ){
        int x = rand() % MAP_SIZE , y = rand() % MAP_SIZE;
        if(map[x][y].mine == true) continue;
        map[x][y].mine = true;
        i++;
    }
    //计算每格周围地雷数量
    for(int x = 0; x < MAP_SIZE; x++){
        for(int y = 0; y < MAP_SIZE ; y++){
            count_around(x, y);
        }
    }
}

static void play_bgm(){
    //判断是否换bgm
    if(!Mix_PlayingMusic()){
        int next_bgm;
        do {
            next_bgm = rand() % BGM_NUM;
        }while(next_bgm == app_game.rand_bgm);
        app_game.rand_bgm = next_bgm;
        play_music(app_game.res.bgm[app_game.rand_bgm].music);
    }
}

static void dead(){
    log("Game over!\n");
}

static void respawn_mine(int x, int y){
    //随机分配周围地雷
    Coord start = {Max(x - 1, 0), Max(y - 1, 0)};
    Coord end = {Min(x + 1, MAP_SIZE - 1), Min(y + 1, MAP_SIZE - 1)};
    int x_spread = end.x - start.x + 1;
    int y_spread = end.y - start.y + 1;
    //清空周围地雷
    for(int i = start.x; i <= end.x; i++){
        for(int j = start.y; j <= end.y; j++){
            if(map[i][j].mine_found || map[i][j].shown) continue;
            if(i == x && j == y)    continue;
            map[i][j].mine = false;
        }
    }
    log("Clear successfully.\n");
    //重分配
    int reassign = map[x][y].mine_around ;
    if(map[x][y].mine && !map[x][y].mine_found){
        reassign--;
    }
    for(int i = 0; i < reassign;){
        int x_mine = rand() % x_spread + start.x, y_mine = rand() % y_spread + start.y;
        if(map[x_mine][y_mine].mine == true || map[x_mine][y_mine].shown == true || map[x_mine][y_mine].mine_found == true ||
                (x_mine == x && y_mine == y) ) continue;
        map[x_mine][y_mine].mine = true;
        i++;
        log("Successfully reassign one mine.\n");
    }
    log("Reassign successfully.\n");
    //重新计算周围地雷数量
    Coord recount_start = {Max(start.x - 1, 0), Max(start.y - 1, 0)};
    Coord recount_end = {Min(end.x + 1, MAP_SIZE - 1), Min(end.y + 1, MAP_SIZE - 1)};
    for(int i = recount_start.x; i <= recount_end.x; i++){
        for(int j = recount_start.y; j <= recount_end.y; j++){
//            if(i == x && j == y) continue;
            count_around(i, j);
            map[i][j].show_mine_around = false;
//            printf("Recount map[%d][%d] successfully, mine_around = %d.\n", i, j, map[i][j].mine_around);
        }
    }
    log("Respawn mine successfully.\n");
}

static void spawn_shrew(){
    //生成地鼠
    unsigned int time = SDL_GetTicks();
    if(time - app_game.time.last_spawn_time >= SHREW_SPAWN_TIME * pow(FASTER, app_game.mine_found)){
        int x, y;
        do{
            x = rand() % MAP_SIZE;
            y = rand() % MAP_SIZE;
        }while(map[x][y].shrew.type != 0);

        int type = rand() % TOTAL_POSSIBILITY;
        if(type >= 0 && type < SHREW_1_POSSIBILITY){
            map[x][y].shrew.type = 1;
        }else if(type >= SHREW_1_POSSIBILITY && type < SHREW_1_POSSIBILITY + SHREW_2_POSSIBILITY){
            map[x][y].shrew.type = 2;
        }else if(type >= SHREW_2_POSSIBILITY && type < TOTAL_POSSIBILITY){
            map[x][y].shrew.type = 3;
        }

        map[x][y].shrew.spawn_time = SDL_GetTicks();

        log("Spawn shrew at (%d, %d), time = %d, type = %d.\n", x, y, map[x][y].shrew.spawn_time, map[x][y].shrew.type);
        play_fx(app_game.res.fx_shrew_laugh, 2);
        app_game.time.last_spawn_time = time;
    }
}

//击中地鼠
static void hit_a_shrew(int x, int y){
    if(map[x][y].shown == false) {
        //根据种类决定操作
        switch (map[x][y].shrew.type) {
            case 1 : {  //普通地鼠
                Coord start = {Max(x - 1, 0), Max(y - 1, 0)};
                Coord end = {Min(x + 1, MAP_SIZE - 1), Min(y + 1, MAP_SIZE - 1)};
                for (int i = start.x; i <= end.x; i++) {
                    for (int j = start.y; j <= end.y; j++) {
                        if (i == x && j == y) {
                            map[i][j].shown = true;
                        } else {
                            map[i][j].show_mine_around = true;
                        }
                    }
                }
                break;
            }
            case 2 : {  //混乱地鼠
                respawn_mine(x, y);
                break;
            }
            case 3: {   //核弹地鼠
                map[x][y].shown = true;
                if (map[x][y].mine == true && map[x][y].mine_found == false) {
                    find_a_mine(x, y);
                    app_game.num_heart -= 5;
                    play_fx(app_game.res.fx_explosion, 4);
                }
                break;
            }
        }
    }
    //清除地鼠
    map[x][y].shrew.type = 0;
    app_game.shrew_hit++;
}

static SDL_Color decide_color(int num){
    SDL_Color ret = {0x00, 0x00, 0x00, 0x00};
    switch(num){
        case 0 :{
            ret = (SDL_Color) EVEN_LIGHT_GREEN;
            break;
        }
        case 1 :{
            ret = (SDL_Color) LIGHT_GREEN;
            break;
        }
        case 2 :{
            ret = (SDL_Color) GREEN;
            break;
        }
        case 3 :{
            ret = (SDL_Color) PINK;
            break;
        }
        case 4 :{
            ret = (SDL_Color) LIGHT_RED;
            break;
        }
        case 5 :{
            ret = (SDL_Color) RED;
            break;
        }
        case 6 :{
            ret = (SDL_Color) DARK_RED;
            break;
        }
        case 7 :{
            ret = (SDL_Color) DARK_PURPLE;
            break;
        }
        case 8 :{
            ret = (SDL_Color) DARK_BLUE;
            break;
        }
        case 9 :{
            ret = (SDL_Color) BLACK;
            break;
        }
        default :
            break;
    }

    return ret;
}

static void find_a_mine(int x, int y){
    //更新map[x][y]中的参数
    map[x][y].shown = true;
    map[x][y].mine_found = true;
    //更新mine
    app_game.mine_found++;
    //更新周围方块的地雷数量信息
    Coord start = {Max(x - 1, 0), Max(y - 1, 0)};
    Coord end = {Min(x + 1, MAP_SIZE - 1), Min(y + 1, MAP_SIZE - 1)};
    for(int i = start.x; i <= end.x; i++){
        for(int j = start.y; j <= end.y; j++){
//            if(i == x && j == y) continue;
            map[i][j].mine_around = Max(0, map[i][j].mine_around - 1);
        }
    }
}

static void count_around(int x, int y) {
    Coord start = {Max(x - 1, 0), Max(y - 1, 0)};
    Coord end = {Min(x + 1, MAP_SIZE - 1), Min(y + 1, MAP_SIZE - 1)};
    int num_mine = 0;
    for (int k = start.x; k <= end.x; k++) {
        for (int l = start.y; l <= end.y; l++) {
            if (map[k][l].mine == true && map[k][l].mine_found == false) {
                num_mine++;
            }
        }
    }
    map[x][y].mine_around = num_mine;
}

static void do_exit_logic(){
    bool win = false;
    if(app_game.num_heart <= 0){
        dead();
    }else if(app_game.mine_found == MINE_NUM){
        log("Win!.\n");
        win = true;
    }else{
        log("Game end by ESC.\n");
    }

    record_game();

    game_end(win);

    quit_game();
}

static void show_map(){
    for(int i = 0; i < MAP_SIZE; i++){
        for(int j = 0; j < MAP_SIZE; j++){
            map[i][j].shown = true;
            map[i][j].shrew.type = 0;
        }
    }
    paint_map();
    display();
    SDL_Delay(2000);
}

static void record_game() {
    // 2024/1/18 修好了这个函数的远古bug
    FILE *file = open_file("./res/file/records.txt", "a+");
    FILE *temp = open_file("./res/file/temp", "w");
    // 读取最多18行
    int line = 0;
    char buffer[90];

    time_t currentTime;
    struct tm *localTime;

    time(&currentTime);
    localTime = localtime(&currentTime);

    // 记录时间
    fprintf(temp, "%s ", asctime(localTime));
    // 记录是否赢了
    if (app_game.mine_found == MINE_NUM) {
        fprintf(temp, "Win. ");
    } else {
        fprintf(temp, "Lose. ");
    }
    // 记录得分
    fprintf(temp, "Mine found: %d, Health: %d, ", app_game.mine_found, app_game.num_heart);
    // 记录用时
    fprintf(temp, "Time %02d:%02d, ", app_game.time.minute, app_game.time.second);
    // 记录打了多少只地鼠
    fprintf(temp, "Shrew hit: %d.\n", app_game.shrew_hit);
    // 逐行读取文件内容
    fseek(file, 0, SEEK_SET);
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        log("temp_text[%d] = %s", line, buffer);
        fputs(buffer, temp);
        line++;
        if (line >= 18) {
            break;
        }
    }

    fclose(file);
    fclose(temp);
    remove("./res/file/records.txt");
    rename("./res/file/temp", "./res/file/records.txt");
    log("Record game successfully.\n");
}

static void quit_game(){
    destroy_graph(app_game.res.background_texture);
    destroy_graph(app_game.res.block_texture);
    destroy_graph(app_game.res.heart_texture);
    destroy_graph(app_game.res.mine_texture);
    destroy_graph(app_game.res.empty_texture);
    destroy_graph(app_game.res.found_texture);
    for(int i = 0; i < 3; i++){
        destroy_graph(app_game.res.shrew_graph[i]);
    }
    //font
    destroy_ttf(app_game.res.text_font);
    destroy_ttf(app_game.res.bgm_font);
    //music
    for(int i = 0; i < BGM_NUM; i++){
        destroy_music(app_game.res.bgm[i].music);
    }
    //chunk
    destroy_fx(app_game.res.fx_hit_shrew);
    destroy_fx(app_game.res.fx_hurt);
    destroy_fx(app_game.res.fx_explosion);
    destroy_fx(app_game.res.fx_found_mine);
    destroy_fx(app_game.res.fx_shrew_laugh);
    log("Release memory successfully.\n");
}

static bool exam_best(){
    FILE *file = open_file("./res/file/best","r+");
    Record best = {0};
    Record record = {'L', app_game.mine_found, app_game.num_heart, app_game.time.minute, app_game.time.second};
    if(app_game.mine_found == 10){
        record.win_or_lose = 'W';
    }
    if(fscanf(file, "%c%d%d%d%d", &best.win_or_lose, &best.mine_found, &best.heart_left, &best.minute, &best.second) == -1){
        fprintf(file, "%c %d %d %d %d\n",record.win_or_lose, record.mine_found, record.heart_left, record.minute, record.second);
        fclose(file);
        return true;
    }

    fseek(file, 0, SEEK_SET);

    if(best.win_or_lose == 'W'){
        if(record.win_or_lose == 'L'){
            fclose(file);
            return false;
        }
        int best_time = best.minute * 60 + best.second;
        int record_time = record.minute * 60 + record.second;
        if(best_time > record_time){
            fprintf(file, "%c %d %d %d %d\n",record.win_or_lose, record.mine_found, record.heart_left, record.minute, record.second);
            fclose(file);
            return true;
        }
    }else if(record.mine_found > best.mine_found){
        fprintf(file, "%c %d %d %d %d\n",record.win_or_lose, record.mine_found, record.heart_left, record.minute, record.second);
        fclose(file);
        return true;
    }
    fclose(file);
    return false;
}

static void game_end(bool win){
    TTF_Font *font = load_text("./res/ttf/GenshinDefault.ttf", 72);
    Coord end_coord = {400, 220};
    Mix_Music *music = load_music("./res/music/launch_pt2.mp3");
    SDL_Texture *background_texture = load_graph("./res/graph/best.jpg");
    Coord null = {-1, -1};
    play_music(music);
    show_map();
    SDL_Color color_title = BLACK;
    color_title.a = 0x00;

    char text[11];
    if(win){
        end_coord.x += 40;
        strcpy(text, "YOU WIN!");
    }else{
        strcpy(text, "GAME OVER!");
    }
    //BEST 渐入
    while(color_title.a < 255) {
        SDL_RenderClear(app.renderer);
        //显示背景图片
        display_graph(background_texture, null);
        //显示标题
        display_text(font, text, color_title, end_coord);
        display();

        color_title.a += 3;
        SDL_Delay(32);
    }

    //检验是否创记录
    if(exam_best()){
        Coord best_coord = {500, 380};
        display_text(app_game.res.text_font, "New record!", (SDL_Color)LIGHT_YELLOW, best_coord);
        log("OK.\n");
        char text_best[20];
        best_coord.y += LINE_SPACE;
        if(app_game.mine_found == MINE_NUM) {
            sprintf(text_best, "Time: %02d:%02d", app_game.time.minute, app_game.time.second);
        }else{
            sprintf(text_best, "FIND %d MINES", app_game.mine_found);
        }
        display_text(app_game.res.text_font, text_best, (SDL_Color)DARK_PURPLE, best_coord);
    }else{
        Coord best_coord = {300, 380};
        char text_best[25];
        FILE *best_file = open_file("./res/file/best","r");
        int num_hit, num_heart, best_minute, best_second;
        char rubbish;
        fscanf(best_file,"%c%d%d%d%d",&rubbish, &num_hit, &num_heart, &best_minute, &best_second);
        sprintf(text_best, "BEST: MINE FOUND: %d, TIME: %02d:%02d",num_hit, best_minute, best_second);
        display_text(app_game.res.text_font, text_best, (SDL_Color)DARK_PURPLE, best_coord);
    }

    TTF_Font *back_to_menu_font = load_text("./res/ttf/Broken.ttf", 48);

    Coord back_to_menu = {440, 560};
    display_text(back_to_menu_font, "BACK TO MENU", color_title, back_to_menu);
    display();

    SDL_Event event;
    int ret = false;
    clean_rubbish_events();
    while(SDL_WaitEvent(&event)){
        do_event(event);

        if(app.keyboard[SDL_SCANCODE_ESCAPE]){
            log("Exited by ESC.\n");
            destroy_graph(background_texture);
            destroy_ttf(back_to_menu_font);
            destroy_ttf(font);
            destroy_music(music);
            exit(EXIT_SUCCESS);
        }

        if(app.keyboard[SDL_SCANCODE_SPACE] || app.keyboard[SDL_SCANCODE_RETURN]){
            log("Back to menu.\n");
            ret = true;
        }

        if(app.left_mouse){
            if(event.button.x >= 500 && event.button.y <= 660 && event.button.y >= 560 && event.button.y <= 610){
                log("Back to menu.\n");
                ret = true;
            }
        }

        if(ret){
            destroy_graph(background_texture);
            destroy_ttf(back_to_menu_font);
            destroy_ttf(font);
            destroy_music(music);
            return;
        }
    }
}