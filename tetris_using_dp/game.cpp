#include <stdio.h>

#include "game.h"
#include "graphic.h"
#include "tetris_logic.h"
#include "control.h"

game* game::ins = NULL;

game::game() {
    logic = tetris_logic::instance();
}

game* game::instance() {
    if(ins == NULL)
        ins = new game();
    return ins;
}

void game::draw() {
    center();
    draw_playgrd();
    draw_preview();
    draw_linerecord();
    draw_blockrecord();
    draw_status();
    draw_cur();
    fflush(stdout);
}

void game::draw_pad(int bc, int n) {
    setcolor(0, bc);
    for(; n>0; --n)
        printf(" ");
    restore();
}

void game::draw_block(int preview, int n) {
    static const int bc[] = {B_BLACK, B_BLACK, B_RED, B_GREEN, B_YELLOW, B_BLUE, B_MAGENTA, B_CYAN, B_WHITE};

    static const char* s[] = {"  ", "  ", "()", "##", "$$", "{}", "<>", "&&", "[]"};

    int f = n==tetris_logic::Z ? F_YELLOW : F_WHITE;
    int b = bc[n];
    if(1 == preview) {
        b = n==tetris_logic::EMPTY ? B_BLUE : B_BLACK;
        f = n==tetris_logic::EMPTY ? F_BLUE : F_BLACK;;
    }
    setcolor(f, b);
    printf("%s", s[n]);
    restore();
}

void game::draw_preview(void) {
    int l,c;
    for(l=0; l<4; ++l) {
        cursor_to(PRV_LEFT, PRV_TOP+l);
        draw_pad(B_BLUE, 1);
        for(c=0; c<4; ++c) {
            cursor_to(PRV_LEFT+c*2+1, PRV_TOP+l);
            draw_block(1, logic->nextgrd[l][c]);
        }
        draw_pad(B_BLUE, 1);
    }
    cursor_to(PRV_LEFT, PRV_TOP+4);
    draw_pad(B_BLUE, 10);
}

void game::draw_playgrd(void) {
    int l,c;
    for(l=0; l<tetris_logic::lines; ++l) {
        for(c=0; c<tetris_logic::cols; ++c) {
            cursor_to(PGRD_LEFT+c*2, PGRD_TOP+l);
            draw_block(0, logic->playgrd[l][c]);
        }
    }
}

void game::draw_cur(void) {
    int i;
    for(i=0; i<4; ++i) {
        if(logic->cur[i].line < 0)
            continue;

        cursor_to(PGRD_LEFT+logic->cur[i].col*2, PGRD_TOP+logic->cur[i].line);
        draw_block(0, logic->curtype);
    }
}

void game::draw_linerecord(void) {
    cursor_to(LR_LEFT, LR_TOP);
    setcolor(F_WHITE, B_BLACK);
    setattr(T_BOLD);
    printf("   LINE   ");
    restore();
    setcolor(F_BLACK, B_WHITE);
    cursor_to(LR_LEFT, LR_TOP+1);
    printf(" 1> %5d ", logic->one);
    cursor_to(LR_LEFT, LR_TOP+2);
    printf(" 2> %5d ", logic->two);
    cursor_to(LR_LEFT, LR_TOP+3);
    printf(" 3> %5d ", logic->three);
    cursor_to(LR_LEFT, LR_TOP+4);
    printf(" 4> %5d ", logic->four);
    cursor_to(LR_LEFT, LR_TOP+5);
    setattr(T_UNDERSCORE);
    printf(" sum %4d ", logic->one+logic->two+logic->three+logic->four);
    restore();
}

void game::draw_blockrecord(void) {
    cursor_to(BR_LEFT, BR_TOP);
    setcolor(F_WHITE, B_BLACK);
    setattr(T_BOLD);
    printf("  BLOCKS  ");
    restore();

    cursor_to(BR_LEFT, BR_TOP+1);
    setcolor(F_WHITE, B_RED);
    printf(" (I) %4d ", logic->i);
    cursor_to(BR_LEFT, BR_TOP+2);
    setcolor(F_WHITE, B_GREEN);
    printf(" #J# %4d ", logic->j);
    cursor_to(BR_LEFT, BR_TOP+3);
    setcolor(F_WHITE, B_YELLOW);
    printf(" $L$ %4d ", logic->l);
    cursor_to(BR_LEFT, BR_TOP+4);
    setcolor(F_WHITE, B_BLUE);
    printf(" {O} %4d ", logic->o);
    cursor_to(BR_LEFT, BR_TOP+5);
    setcolor(F_WHITE, B_MAGENTA);
    printf(" <S> %4d ", logic->s);
    cursor_to(BR_LEFT, BR_TOP+6);
    setcolor(F_WHITE, B_CYAN);
    printf(" &T& %4d ", logic->t);
    cursor_to(BR_LEFT, BR_TOP+7);
    setcolor(F_YELLOW, B_WHITE);
    printf(" [Z] %4d ", logic->z);
    cursor_to(BR_LEFT, BR_TOP+8);
    setcolor(F_WHITE, B_BLACK);
    setattr(T_UNDERSCORE);
    printf(" sum %4d ", logic->i+logic->j+logic->l+logic->o+logic->s+logic->t+logic->z);
    restore();
}

void game::draw_status(void) {
    cursor_to(ST_LEFT, ST_TOP);
    setcolor(F_WHITE, B_BLACK);
    setattr(T_BOLD);
    printf("  LEVEL  ");
    restore();
    setcolor(F_BLACK, B_WHITE);
    cursor_to(ST_LEFT, ST_TOP+1);
    printf("%8d ", logic->level+1);

    cursor_to(ST_LEFT, ST_TOP+3);
    setcolor(F_WHITE, B_BLACK);
    setattr(T_BOLD);
    printf("  SCORE  ");
    restore();
    setcolor(F_BLACK, B_WHITE);
    cursor_to(ST_LEFT, ST_TOP+4);
    printf("%8d ", logic->score);

    cursor_to(ST_LEFT, ST_TOP+6);
    setcolor(F_WHITE, B_BLACK);
    setattr(T_BOLD);
    printf("  SPEED  ");
    restore();
    setcolor(F_BLACK, B_WHITE);
    cursor_to(ST_LEFT, ST_TOP+7);
    printf(" %7.2f ", 1000.0f/speeds[logic->level]);

    cursor_to(ST_LEFT, ST_TOP+9);
    setcolor(F_WHITE, B_BLACK);
    setattr(T_BOLD);
    printf("NEXTLEVEL");
    restore();
    setcolor(F_BLACK, B_WHITE);
    cursor_to(ST_LEFT, ST_TOP+10);
    printf("%8d ", levels[logic->level]-logic->score);

    restore();
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
