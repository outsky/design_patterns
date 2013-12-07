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

void game::draw_block(int preview, tetris_logic::TYPE type) {
    int f = type==tetris_logic::Z ? graphic::F_YELLOW : graphic::F_WHITE;
    int b = bg_color[type];
    if(1 == preview) {
        b = type==tetris_logic::EMPTY ? graphic::B_BLUE : graphic::B_BLACK;
        f = type==tetris_logic::EMPTY ? graphic::F_BLUE : graphic::F_BLACK;;
    }
    graphic::setcolor(f, b);
    printf("%s", fill_str[type]);
    graphic::restore();
}

void game::draw_preview(void) {
    int l,c;
    for(l=0; l<4; ++l) {
        control::instance()->cursor_to(PRV_LEFT, PRV_TOP+l);
        draw_pad(graphic::B_BLUE, 1);
        for(c=0; c<4; ++c) {
            control::instance()->cursor_to(PRV_LEFT+c*2+1, PRV_TOP+l);
            draw_block(1, logic->nextgrd[l][c]);
        }
        draw_pad(graphic::B_BLUE, 1);
    }
    control::instance()->cursor_to(PRV_LEFT, PRV_TOP+4);
    draw_pad(graphic::B_BLUE, 10);
}

void game::draw_playgrd(void) {
    int l,c;
    for(l=0; l<tetris_logic::lines; ++l) {
        for(c=0; c<tetris_logic::cols; ++c) {
            control::instance()->cursor_to(PGRD_LEFT+c*2, PGRD_TOP+l);
            draw_block(0, logic->playgrd[l][c]);
        }
    }
}

void game::draw_cur(void) {
    int i;
    for(i=0; i<4; ++i) {
        if(logic->cur->b[i].line < 0)
            continue;

        control::instance()->cursor_to(PGRD_LEFT+logic->cur->b[i].col*2, PGRD_TOP+logic->cur->b[i].line);
        draw_block(0, tetris_logic::get_type(logic->cur->type));
    }
}

void game::draw_linerecord(void) {
    control::instance()->cursor_to(LR_LEFT, LR_TOP);
    graphic::setcolor(graphic::F_WHITE, graphic::B_BLACK);
    graphic::setattr(graphic::T_BOLD);
    printf("   LINE   ");
    graphic::restore();
    graphic::setcolor(graphic::F_BLACK, graphic::B_WHITE);
    control::instance()->cursor_to(LR_LEFT, LR_TOP+1);
    printf(" 1> %5d ", logic->one);
    control::instance()->cursor_to(LR_LEFT, LR_TOP+2);
    printf(" 2> %5d ", logic->two);
    control::instance()->cursor_to(LR_LEFT, LR_TOP+3);
    printf(" 3> %5d ", logic->three);
    control::instance()->cursor_to(LR_LEFT, LR_TOP+4);
    printf(" 4> %5d ", logic->four);
    control::instance()->cursor_to(LR_LEFT, LR_TOP+5);
    graphic::setattr(graphic::T_UNDERSCORE);
    printf(" sum %4d ", logic->one+logic->two+logic->three+logic->four);
    graphic::restore();
}

void game::draw_blockrecord(void) {
    control::instance()->cursor_to(BR_LEFT, BR_TOP);
    graphic::setcolor(graphic::F_WHITE, graphic::B_BLACK);
    graphic::setattr(graphic::T_BOLD);
    printf("  BLOCKS  ");
    graphic::restore();

    control::instance()->cursor_to(BR_LEFT, BR_TOP+1);
    graphic::setcolor(graphic::F_WHITE, graphic::B_RED);
    printf(" (I) %4d ", logic->i);
    control::instance()->cursor_to(BR_LEFT, BR_TOP+2);
    graphic::setcolor(graphic::F_WHITE, graphic::B_GREEN);
    printf(" #J# %4d ", logic->j);
    control::instance()->cursor_to(BR_LEFT, BR_TOP+3);
    graphic::setcolor(graphic::F_WHITE, graphic::B_YELLOW);
    printf(" $L$ %4d ", logic->l);
    control::instance()->cursor_to(BR_LEFT, BR_TOP+4);
    graphic::setcolor(graphic::F_WHITE, graphic::B_BLUE);
    printf(" {O} %4d ", logic->o);
    control::instance()->cursor_to(BR_LEFT, BR_TOP+5);
    graphic::setcolor(graphic::F_WHITE, graphic::B_MAGENTA);
    printf(" <S> %4d ", logic->s);
    control::instance()->cursor_to(BR_LEFT, BR_TOP+6);
    graphic::setcolor(graphic::F_WHITE, graphic::B_CYAN);
    printf(" &T& %4d ", logic->t);
    control::instance()->cursor_to(BR_LEFT, BR_TOP+7);
    graphic::setcolor(graphic::F_YELLOW, graphic::B_WHITE);
    printf(" [Z] %4d ", logic->z);
    control::instance()->cursor_to(BR_LEFT, BR_TOP+8);
    graphic::setcolor(graphic::F_WHITE, graphic::B_BLACK);
    graphic::setattr(graphic::T_UNDERSCORE);
    printf(" sum %4d ", logic->i+logic->j+logic->l+logic->o+logic->s+logic->t+logic->z);
    graphic::restore();
}

void game::draw_status(void) {
    control::instance()->cursor_to(ST_LEFT, ST_TOP);
    graphic::setcolor(graphic::F_WHITE, graphic::B_BLACK);
    graphic::setattr(graphic::T_BOLD);
    printf("  LEVEL  ");
    graphic::restore();
    graphic::setcolor(graphic::F_BLACK, graphic::B_WHITE);
    control::instance()->cursor_to(ST_LEFT, ST_TOP+1);
    printf("%8d ", logic->level+1);

    control::instance()->cursor_to(ST_LEFT, ST_TOP+3);
    graphic::setcolor(graphic::F_WHITE, graphic::B_BLACK);
    graphic::setattr(graphic::T_BOLD);
    printf("  SCORE  ");
    graphic::restore();
    graphic::setcolor(graphic::F_BLACK, graphic::B_WHITE);
    control::instance()->cursor_to(ST_LEFT, ST_TOP+4);
    printf("%8d ", logic->score);

    control::instance()->cursor_to(ST_LEFT, ST_TOP+6);
    graphic::setcolor(graphic::F_WHITE, graphic::B_BLACK);
    graphic::setattr(graphic::T_BOLD);
    printf("  SPEED  ");
    graphic::restore();
    graphic::setcolor(graphic::F_BLACK, graphic::B_WHITE);
    control::instance()->cursor_to(ST_LEFT, ST_TOP+7);
    printf(" %7.2f ", 1000.0f/speeds[logic->level]);

    control::instance()->cursor_to(ST_LEFT, ST_TOP+9);
    graphic::setcolor(graphic::F_WHITE, graphic::B_BLACK);
    graphic::setattr(graphic::T_BOLD);
    printf("NEXTLEVEL");
    graphic::restore();
    graphic::setcolor(graphic::F_BLACK, graphic::B_WHITE);
    control::instance()->cursor_to(ST_LEFT, ST_TOP+10);
    printf("%8d ", levels[logic->level]-logic->score);

    graphic::restore();
}

void game::move_down() {
    logic->move_down();
}

void game::move_left() {
    logic->move_left();
}

void game::move_right() {
    logic->move_right();
}

void game::drop_down() {
    logic->drop_down();
}

void game::rotate() {
    logic->rotate();
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
    control::instance()->prepare_input();
    graphic::erase_display();

    draw();
}

void game::quit() {
    graphic::restore();
    control::instance()->restore_input();
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
            game::instance()->move_down();
        else if(c == 'k')
            game::instance()->rotate();
        else if(c == 'h')
            game::instance()-> move_left();
        else if(c == 'l')
            game::instance()->move_right();
        else if(c == 32)
            game::instance()->drop_down();
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
            ins->move_down();
            t->reset();
        }
        usleep(50);
    }
    return NULL;
}
