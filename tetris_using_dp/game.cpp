#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

#include "ansi_escape.h"
#include "game.h"
#include "graphic.h"
#include "tetris_logic.h"

game* game::ins = NULL;

game::game() {
    PGRD_LEFT = 10;
    PGRD_TOP = 5;
    PRV_LEFT = 0;
    PRV_TOP = 0;
    ST_LEFT = 0;
    ST_TOP = 0;
    LR_LEFT = 0;
    LR_TOP = 0;
    BR_LEFT = 0;
    BR_TOP = 0;

    bg_color[tetris_logic::EMPTY] = ansi_escape::B_BLACK;
    bg_color[tetris_logic::ACTIVE] = ansi_escape::B_BLACK;
    bg_color[tetris_logic::I] = ansi_escape::B_RED;
    bg_color[tetris_logic::J] = ansi_escape::B_GREEN;
    bg_color[tetris_logic::L] = ansi_escape::B_YELLOW;
    bg_color[tetris_logic::O] = ansi_escape::B_BLUE;
    bg_color[tetris_logic::S] = ansi_escape::B_MAGENTA;
    bg_color[tetris_logic::T] = ansi_escape::B_CYAN;
    bg_color[tetris_logic::Z] = ansi_escape::B_WHITE;

    fill_str[tetris_logic::EMPTY] = "  ";
    fill_str[tetris_logic::ACTIVE] = "  ";
    fill_str[tetris_logic::I] = "()";
    fill_str[tetris_logic::J] = "##";
    fill_str[tetris_logic::L] = "$$";
    fill_str[tetris_logic::O] = "{}";
    fill_str[tetris_logic::S] = "<>";
    fill_str[tetris_logic::T] = "&&";
    fill_str[tetris_logic::Z] = "[]";

    logic = tetris_logic::instance();
    tm = timer::instance();
}

game* game::instance() {
    if(ins == NULL)
        ins = new game();
    return ins;
}

void game::draw() {
    adjust_position();
    draw_playgrd();
    draw_preview();
    draw_linerecord();
    draw_blockrecord();
    draw_status();
    draw_cur();
    fflush(stdout);
}

void game::draw_pad(int bc, int n) {
    graphic::set_color(0, bc);
    for(; n>0; --n)
        printf(" ");
    graphic::reset_color();
}

void game::draw_block(bool preview, tetris_logic::TYPE type) {
    int f = type==tetris_logic::Z ? ansi_escape::F_YELLOW : ansi_escape::F_WHITE;
    int b = bg_color[type];
    if( preview ) {
        b = type==tetris_logic::EMPTY ? ansi_escape::B_BLUE : ansi_escape::B_BLACK;
        f = type==tetris_logic::EMPTY ? ansi_escape::F_BLUE : ansi_escape::F_BLACK;;
    }
    graphic::draw_text(f, b, "%s", fill_str[type]);
    //graphic::draw_text(f, b, "  ");
}

void game::draw_preview(void) {
    for(int l=0; l<4; ++l) {
        ansi_escape::cursor_to(PRV_LEFT, PRV_TOP+l);
        draw_pad(ansi_escape::B_BLUE, 1);
        for(int c=0; c<4; ++c) {
            ansi_escape::cursor_to(PRV_LEFT+c*2+1, PRV_TOP+l);
            draw_block(true, logic->nextgrd[l][c]);
        }
        draw_pad(ansi_escape::B_BLUE, 1);
    }
    ansi_escape::cursor_to(PRV_LEFT, PRV_TOP+4);
    draw_pad(ansi_escape::B_BLUE, 10);
}

void game::draw_playgrd(void) {
    int l,c;
    for(l=0; l<tetris_logic::lines; ++l) {
        for(c=0; c<tetris_logic::cols; ++c) {
            ansi_escape::cursor_to(PGRD_LEFT+c*2, PGRD_TOP+l);
            draw_block(false, logic->playgrd[l][c]);
        }
    }
}

void game::draw_cur(void) {
    for(int i=0; i<4; ++i) {
        if(logic->cur->b[i].line < 0)
            continue;

        ansi_escape::cursor_to(PGRD_LEFT+logic->cur->b[i].col*2, PGRD_TOP+logic->cur->b[i].line);
        draw_block(false, tetris_logic::get_type(logic->cur->type));
    }
}

void game::draw_linerecord(void) {
    ansi_escape::cursor_to(LR_LEFT, LR_TOP);
    ansi_escape::setattr(ansi_escape::T_BOLD);
    graphic::draw_text(ansi_escape::F_WHITE, ansi_escape::B_BLACK, "   LINE   ");
    ansi_escape::setcolor(ansi_escape::F_BLACK, ansi_escape::B_WHITE);
    ansi_escape::cursor_to(LR_LEFT, LR_TOP+1);
    printf(" 1> %5d ", logic->one);
    ansi_escape::cursor_to(LR_LEFT, LR_TOP+2);
    printf(" 2> %5d ", logic->two);
    ansi_escape::cursor_to(LR_LEFT, LR_TOP+3);
    printf(" 3> %5d ", logic->three);
    ansi_escape::cursor_to(LR_LEFT, LR_TOP+4);
    printf(" 4> %5d ", logic->four);
    ansi_escape::cursor_to(LR_LEFT, LR_TOP+5);
    ansi_escape::setattr(ansi_escape::T_UNDERSCORE);
    printf(" sum %4d ", logic->one+logic->two+logic->three+logic->four);
    graphic::reset_color();
}

void game::draw_blockrecord(void) {
    ansi_escape::cursor_to(BR_LEFT, BR_TOP);
    ansi_escape::setcolor(ansi_escape::F_WHITE, ansi_escape::B_BLACK);
    ansi_escape::setattr(ansi_escape::T_BOLD);
    printf("  BLOCKS  ");
    graphic::reset_color();

    ansi_escape::cursor_to(BR_LEFT, BR_TOP+1);
    if(logic->cur->type == 'I')
        ansi_escape::setcolor(ansi_escape::F_WHITE, ansi_escape::B_RED);
    printf(" (I) %4d ", logic->i);
    graphic::reset_color();

    ansi_escape::cursor_to(BR_LEFT, BR_TOP+2);
    if(logic->cur->type == 'J')
        ansi_escape::setcolor(ansi_escape::F_WHITE, ansi_escape::B_GREEN);
    printf(" #J# %4d ", logic->j);
    graphic::reset_color();

    ansi_escape::cursor_to(BR_LEFT, BR_TOP+3);
    if(logic->cur->type == 'L')
        ansi_escape::setcolor(ansi_escape::F_WHITE, ansi_escape::B_YELLOW);
    printf(" $L$ %4d ", logic->l);
    graphic::reset_color();

    ansi_escape::cursor_to(BR_LEFT, BR_TOP+4);
    if(logic->cur->type == 'O')
        ansi_escape::setcolor(ansi_escape::F_WHITE, ansi_escape::B_BLUE);
    printf(" {O} %4d ", logic->o);
    graphic::reset_color();

    ansi_escape::cursor_to(BR_LEFT, BR_TOP+5);
    if(logic->cur->type == 'S')
        ansi_escape::setcolor(ansi_escape::F_WHITE, ansi_escape::B_MAGENTA);
    printf(" <S> %4d ", logic->s);
    graphic::reset_color();

    ansi_escape::cursor_to(BR_LEFT, BR_TOP+6);
    if(logic->cur->type == 'T')
        ansi_escape::setcolor(ansi_escape::F_WHITE, ansi_escape::B_CYAN);
    printf(" &T& %4d ", logic->t);
    graphic::reset_color();

    ansi_escape::cursor_to(BR_LEFT, BR_TOP+7);
    if(logic->cur->type == 'Z')
        ansi_escape::setcolor(ansi_escape::F_YELLOW, ansi_escape::B_WHITE);
    printf(" [Z] %4d ", logic->z);
    graphic::reset_color();

    ansi_escape::cursor_to(BR_LEFT, BR_TOP+8);
    ansi_escape::setcolor(ansi_escape::F_WHITE, ansi_escape::B_BLACK);
    ansi_escape::setattr(ansi_escape::T_UNDERSCORE);
    printf(" sum %4d ", logic->i+logic->j+logic->l+logic->o+logic->s+logic->t+logic->z);
    graphic::reset_color();
}

void game::draw_status(void) {
    ansi_escape::cursor_to(ST_LEFT, ST_TOP);
    ansi_escape::setcolor(ansi_escape::F_WHITE, ansi_escape::B_BLACK);
    ansi_escape::setattr(ansi_escape::T_BOLD);
    printf("  LEVEL  ");
    graphic::reset_color();
    ansi_escape::setcolor(ansi_escape::F_BLACK, ansi_escape::B_WHITE);
    ansi_escape::cursor_to(ST_LEFT, ST_TOP+1);
    printf("%8d ", logic->level+1);

    ansi_escape::cursor_to(ST_LEFT, ST_TOP+3);
    ansi_escape::setcolor(ansi_escape::F_WHITE, ansi_escape::B_BLACK);
    ansi_escape::setattr(ansi_escape::T_BOLD);
    printf("  SCORE  ");
    graphic::reset_color();
    ansi_escape::setcolor(ansi_escape::F_BLACK, ansi_escape::B_WHITE);
    ansi_escape::cursor_to(ST_LEFT, ST_TOP+4);
    printf("%8d ", logic->score);

    ansi_escape::cursor_to(ST_LEFT, ST_TOP+6);
    ansi_escape::setcolor(ansi_escape::F_WHITE, ansi_escape::B_BLACK);
    ansi_escape::setattr(ansi_escape::T_BOLD);
    printf("  SPEED  ");
    graphic::reset_color();
    ansi_escape::setcolor(ansi_escape::F_BLACK, ansi_escape::B_WHITE);
    ansi_escape::cursor_to(ST_LEFT, ST_TOP+7);
    printf(" %7.2f ", 1000.0f/speeds[logic->level]);

    ansi_escape::cursor_to(ST_LEFT, ST_TOP+9);
    ansi_escape::setcolor(ansi_escape::F_WHITE, ansi_escape::B_BLACK);
    ansi_escape::setattr(ansi_escape::T_BOLD);
    printf("NEXTLEVEL");
    graphic::reset_color();
    ansi_escape::setcolor(ansi_escape::F_BLACK, ansi_escape::B_WHITE);
    ansi_escape::cursor_to(ST_LEFT, ST_TOP+10);
    printf("%8d ", levels[logic->level]-logic->score);

    graphic::reset_color();
}

void game::adjust_position() {
    static struct winsize old;
    struct winsize ws;
    if(-1==ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) ||
      0==memcmp(&old, &ws, sizeof(struct winsize)))
        return;

    PGRD_LEFT = (ws.ws_col-tetris_logic::cols*2)/2;
    PGRD_TOP = (ws.ws_row-tetris_logic::lines)/2;
    PRV_LEFT = PGRD_LEFT+tetris_logic::cols*2+1;
    PRV_TOP = PGRD_TOP;
    ST_LEFT = PGRD_LEFT-9;
    ST_TOP = PGRD_TOP;
    BR_LEFT = PRV_LEFT;
    BR_TOP = PGRD_TOP+tetris_logic::lines-9;
    LR_LEFT = PRV_LEFT;
    LR_TOP = BR_TOP-6;

    memcpy(&old, &ws, sizeof(struct winsize));
    ansi_escape::erase_display();
}

void game::prepare() {
    quit_handler_init();
    prepare_input();
    ansi_escape::erase_display();

    draw();
}

void game::quit() {
    graphic::reset_color();
    restore_input();
    ansi_escape::erase_display();
    logic->destroy();
    printf("\n");
}

void game::run() {
    pthread_t tid;
    pthread_create(&tid, NULL, trd_timer, NULL);
    pthread_detach(tid);
    pthread_create(&tid, NULL, trd_draw, NULL);
    pthread_detach(tid);

    int c;
    while(EOF != (c=getchar())) {
        if(c == 'j')
            logic->move_down();
        else if(c == 'k')
            logic->rotate();
        else if(c == 'h')
            logic->move_left();
        else if(c == 'l')
            logic->move_right();
        else if(c == 32)
            logic->drop_down();
        else if(c == 'q')
            break;
        else
            continue;
    }
}

void game::quit_handler_init() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = sigint;
    sigaction(SIGINT, &sa, NULL);
    atexit(on_quit);
}

void game::on_quit(void) {
    instance()->quit();
    exit(0);
}

void game::sigint(int n) {
    on_quit();
}

void* game::trd_draw(void* p) {
    static tetris_logic* l = tetris_logic::instance();
    for(;;) {
        if(0 == pthread_mutex_lock(&l->mut)) {
            pthread_cond_wait(&l->cond, &l->mut);
            ins->draw();
            pthread_mutex_unlock(&l->mut);
        }
    }
    return NULL;
}

void* game::trd_timer(void* p) {
    static timer* t = timer::instance();
    static tetris_logic* l = tetris_logic::instance();
    for(;;) {
        timer::instance()->update();
        if(t->get_interval() >= speeds[l->level]) {
            tetris_logic::instance()->move_down();
            t->reset();
        }
        usleep(50);
    }
    return NULL;
}

void game::prepare_input() {
    struct termios tm;
    tcgetattr(0, &tm_org);
    tm = tm_org;
    tm.c_lflag &= (~ICANON);
    tm.c_lflag &= (~(ECHO|ECHOE|ECHOK|ECHONL));
    tm.c_cc[VTIME] = 0;
    tm.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &tm);

    ansi_escape::cursor_hide();
}

void game::restore_input() {
    tcsetattr(0, TCSANOW, &tm_org);
    ansi_escape::cursor_show();
}
