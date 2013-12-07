#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

#include "game.h"
#include "graphic.h"
#include "tetris_logic.h"
#include "control.h"

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

    bg_color[tetris_logic::EMPTY] = graphic::B_BLACK;
    bg_color[tetris_logic::ACTIVE] = graphic::B_BLACK;
    bg_color[tetris_logic::I] = graphic::B_RED;
    bg_color[tetris_logic::J] = graphic::B_GREEN;
    bg_color[tetris_logic::L] = graphic::B_YELLOW;
    bg_color[tetris_logic::O] = graphic::B_BLUE;
    bg_color[tetris_logic::S] = graphic::B_MAGENTA;
    bg_color[tetris_logic::T] = graphic::B_CYAN;
    bg_color[tetris_logic::Z] = graphic::B_WHITE;

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
    ctrl = control::instance();
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
    graphic::setcolor(0, bc);
    for(; n>0; --n)
        printf(" ");
    graphic::restore();
}

void game::draw_block(bool preview, tetris_logic::TYPE type) {
    int f = type==tetris_logic::Z ? graphic::F_YELLOW : graphic::F_WHITE;
    int b = bg_color[type];
    if( preview ) {
        b = type==tetris_logic::EMPTY ? graphic::B_BLUE : graphic::B_BLACK;
        f = type==tetris_logic::EMPTY ? graphic::F_BLUE : graphic::F_BLACK;;
    }
    graphic::setcolor(f, b);
    printf("%s", fill_str[type]);
    graphic::restore();
}

void game::draw_preview(void) {
    for(int l=0; l<4; ++l) {
        ctrl->cursor_to(PRV_LEFT, PRV_TOP+l);
        draw_pad(graphic::B_BLUE, 1);
        for(int c=0; c<4; ++c) {
            ctrl->cursor_to(PRV_LEFT+c*2+1, PRV_TOP+l);
            draw_block(true, logic->nextgrd[l][c]);
        }
        draw_pad(graphic::B_BLUE, 1);
    }
    ctrl->cursor_to(PRV_LEFT, PRV_TOP+4);
    draw_pad(graphic::B_BLUE, 10);
}

void game::draw_playgrd(void) {
    int l,c;
    for(l=0; l<tetris_logic::lines; ++l) {
        for(c=0; c<tetris_logic::cols; ++c) {
            ctrl->cursor_to(PGRD_LEFT+c*2, PGRD_TOP+l);
            draw_block(false, logic->playgrd[l][c]);
        }
    }
}

void game::draw_cur(void) {
    for(int i=0; i<4; ++i) {
        if(logic->cur->b[i].line < 0)
            continue;

        ctrl->cursor_to(PGRD_LEFT+logic->cur->b[i].col*2, PGRD_TOP+logic->cur->b[i].line);
        draw_block(false, tetris_logic::get_type(logic->cur->type));
    }
}

void game::draw_linerecord(void) {
    ctrl->cursor_to(LR_LEFT, LR_TOP);
    graphic::setcolor(graphic::F_WHITE, graphic::B_BLACK);
    graphic::setattr(graphic::T_BOLD);
    printf("   LINE   ");
    graphic::restore();
    graphic::setcolor(graphic::F_BLACK, graphic::B_WHITE);
    ctrl->cursor_to(LR_LEFT, LR_TOP+1);
    printf(" 1> %5d ", logic->one);
    ctrl->cursor_to(LR_LEFT, LR_TOP+2);
    printf(" 2> %5d ", logic->two);
    ctrl->cursor_to(LR_LEFT, LR_TOP+3);
    printf(" 3> %5d ", logic->three);
    ctrl->cursor_to(LR_LEFT, LR_TOP+4);
    printf(" 4> %5d ", logic->four);
    ctrl->cursor_to(LR_LEFT, LR_TOP+5);
    graphic::setattr(graphic::T_UNDERSCORE);
    printf(" sum %4d ", logic->one+logic->two+logic->three+logic->four);
    graphic::restore();
}

void game::draw_blockrecord(void) {
    ctrl->cursor_to(BR_LEFT, BR_TOP);
    graphic::setcolor(graphic::F_WHITE, graphic::B_BLACK);
    graphic::setattr(graphic::T_BOLD);
    printf("  BLOCKS  ");
    graphic::restore();

    ctrl->cursor_to(BR_LEFT, BR_TOP+1);
    if(logic->cur->type == 'I')
        graphic::setcolor(graphic::F_WHITE, graphic::B_RED);
    printf(" (I) %4d ", logic->i);
    graphic::restore();

    ctrl->cursor_to(BR_LEFT, BR_TOP+2);
    if(logic->cur->type == 'J')
        graphic::setcolor(graphic::F_WHITE, graphic::B_GREEN);
    printf(" #J# %4d ", logic->j);
    graphic::restore();

    ctrl->cursor_to(BR_LEFT, BR_TOP+3);
    if(logic->cur->type == 'L')
        graphic::setcolor(graphic::F_WHITE, graphic::B_YELLOW);
    printf(" $L$ %4d ", logic->l);
    graphic::restore();

    ctrl->cursor_to(BR_LEFT, BR_TOP+4);
    if(logic->cur->type == 'O')
        graphic::setcolor(graphic::F_WHITE, graphic::B_BLUE);
    printf(" {O} %4d ", logic->o);
    graphic::restore();

    ctrl->cursor_to(BR_LEFT, BR_TOP+5);
    if(logic->cur->type == 'S')
        graphic::setcolor(graphic::F_WHITE, graphic::B_MAGENTA);
    printf(" <S> %4d ", logic->s);
    graphic::restore();

    ctrl->cursor_to(BR_LEFT, BR_TOP+6);
    if(logic->cur->type == 'T')
        graphic::setcolor(graphic::F_WHITE, graphic::B_CYAN);
    printf(" &T& %4d ", logic->t);
    graphic::restore();

    ctrl->cursor_to(BR_LEFT, BR_TOP+7);
    if(logic->cur->type == 'Z')
        graphic::setcolor(graphic::F_YELLOW, graphic::B_WHITE);
    printf(" [Z] %4d ", logic->z);
    graphic::restore();

    ctrl->cursor_to(BR_LEFT, BR_TOP+8);
    graphic::setcolor(graphic::F_WHITE, graphic::B_BLACK);
    graphic::setattr(graphic::T_UNDERSCORE);
    printf(" sum %4d ", logic->i+logic->j+logic->l+logic->o+logic->s+logic->t+logic->z);
    graphic::restore();
}

void game::draw_status(void) {
    ctrl->cursor_to(ST_LEFT, ST_TOP);
    graphic::setcolor(graphic::F_WHITE, graphic::B_BLACK);
    graphic::setattr(graphic::T_BOLD);
    printf("  LEVEL  ");
    graphic::restore();
    graphic::setcolor(graphic::F_BLACK, graphic::B_WHITE);
    ctrl->cursor_to(ST_LEFT, ST_TOP+1);
    printf("%8d ", logic->level+1);

    ctrl->cursor_to(ST_LEFT, ST_TOP+3);
    graphic::setcolor(graphic::F_WHITE, graphic::B_BLACK);
    graphic::setattr(graphic::T_BOLD);
    printf("  SCORE  ");
    graphic::restore();
    graphic::setcolor(graphic::F_BLACK, graphic::B_WHITE);
    ctrl->cursor_to(ST_LEFT, ST_TOP+4);
    printf("%8d ", logic->score);

    ctrl->cursor_to(ST_LEFT, ST_TOP+6);
    graphic::setcolor(graphic::F_WHITE, graphic::B_BLACK);
    graphic::setattr(graphic::T_BOLD);
    printf("  SPEED  ");
    graphic::restore();
    graphic::setcolor(graphic::F_BLACK, graphic::B_WHITE);
    ctrl->cursor_to(ST_LEFT, ST_TOP+7);
    printf(" %7.2f ", 1000.0f/speeds[logic->level]);

    ctrl->cursor_to(ST_LEFT, ST_TOP+9);
    graphic::setcolor(graphic::F_WHITE, graphic::B_BLACK);
    graphic::setattr(graphic::T_BOLD);
    printf("NEXTLEVEL");
    graphic::restore();
    graphic::setcolor(graphic::F_BLACK, graphic::B_WHITE);
    ctrl->cursor_to(ST_LEFT, ST_TOP+10);
    printf("%8d ", levels[logic->level]-logic->score);

    graphic::restore();
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
    graphic::erase_display();
}

void game::prepare() {
    quit_handler_init();
    ctrl->prepare_input();
    graphic::erase_display();

    draw();
}

void game::quit() {
    graphic::restore();
    ctrl->restore_input();
    graphic::erase_display();
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
