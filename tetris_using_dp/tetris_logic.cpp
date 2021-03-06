#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "tetris_logic.h"
#include "./block/block_i.h"
#include "./block/block_j.h"
#include "./block/block_l.h"
#include "./block/block_o.h"
#include "./block/block_s.h"
#include "./block/block_t.h"
#include "./block/block_z.h"

#define MAXLEVEL 11
int levels[MAXLEVEL] = {25, 50, 100, 180, 280, 400, 550, 700, 900, 1200, 1500};
static int scores[4] = {1, 5, 10, 20};
int speeds[MAXLEVEL] = {1000, 950, 900, 850, 800, 750, 700, 650, 600, 550, 500};

tetris_logic* tetris_logic::ins = NULL;

tetris_logic::tetris_logic() {
    pthread_mutex_init(&mut, NULL);
    pthread_cond_init(&cond, NULL);

    cur = NULL;
    memset(playgrd, EMPTY, sizeof(char)*cols*lines);

    srand(time(NULL));
    nexttyp = rand()%7 + 2;
    nextstate = rand()%4;
    next();
}

tetris_logic* tetris_logic::instance() {
    if(ins == NULL)
        ins = new tetris_logic();
    return ins;
}

void tetris_logic::destroy() {
    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&cond);
}

bool tetris_logic::can_move_down() {
    for(int i=0; i<4; ++i) {
        block::pos* c = &cur->b[i];
        if(c->line+1>=lines || (c->line+1>=0 && playgrd[c->line+1][c->col]!=EMPTY))
            return false;
    }
    return true;
}

bool tetris_logic::can_move_left() {
    for(int i=0; i<4; ++i) {
        block::pos* c = &cur->b[i];
        if(c->col-1<0 || (c->line>=0 && playgrd[c->line][c->col-1]!=EMPTY))
            return false;
    }
    return true;
}

bool tetris_logic::can_move_right() {
    for(int i=0; i<4; ++i) {
        block::pos* c = &cur->b[i];
        if(c->col+1>=cols || (c->line>=0 && playgrd[c->line][c->col+1]!=EMPTY))
            return false;
    }
    return true;
}

int tetris_logic::drop_down(void) {
    while(1 == move_down())
        ;
    return 0;
}

int tetris_logic::move_down() {
    if( !can_move_down() ) {
        stick();
        if( isgameover() ) {
            gameover();
            return 0;
        }
        int ln = clearlines();
        onclearline(ln);
        next();
        return 0;
    }

    pthread_mutex_lock(&mut);
    cur->move_down();
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mut);

    return 1;
}

int tetris_logic::move_right() {
    if( !can_move_right() )
        return 0;

    pthread_mutex_lock(&mut);
    cur->move_right();
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mut);

    return 1;
}

int tetris_logic::move_left() {
    if( !can_move_left() )
        return 0;

    pthread_mutex_lock(&mut);
    cur->move_left();
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mut);

    return 1;
}

int tetris_logic::rotate() {
    pthread_mutex_lock(&mut);
    int ret = 0;
    switch(get_type(cur->type)) {
        case I:
        ret = rotate_i();
        break;

        case J:
        ret = rotate_j();
        break;

        case L:
        ret = rotate_l();
        break;

        case S:
        ret = rotate_s();
        break;

        case T:
        ret = rotate_t();
        break;

        case Z:
        ret = rotate_z();
        break;

        case O:
        default:
        break;
    }

    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mut);
    return ret;
}

// FOR ALL rotate_x FUNCTIONS
//
// c  : the center block
// num: the index of block
// {} : next state blocks
// [] : current state blocks
// <> : blocks
// #  : this block blocks the rotation
int tetris_logic::rotate_i(void) {
    block::pos* c = cur->b;
    int cl = c[1].line;
    int cc = c[1].col;
    int nextstate = (cur->state+1)%4;
    switch(nextstate) {
        /*
            <#><#>{#}
            [3][2][c][0]
                  {#}<#>
                  {#}
        */
        case 0:
        case 2:
            if(!isempty(cl-1,cc-2) || !isempty(cl-1,cc-1) || !isempty(cl-1,cc) ||
              !isempty(cl+1,cc) || !isempty(cl+1,cc+1) || !isempty(cl+2,cc))
                return 0;
            break;

        /*
                [0]<#>
          {#}{#}[c]{#}
             <#>[2]
             <#>[3]
        */
        case 1:
        case 3:
            if(!isempty(cl-1,cc+1) || !isempty(cl+1,cc-1) || !isempty(cl+2,cc-1) ||
              !isempty(cl,cc-2) || !isempty(cl,cc-1) || !isempty(cl,cc+1))
                return 0;
            break;
    }
    cur->rotate();
    return 1;
}

int tetris_logic::rotate_j(void) {
    int cl = cur->b[1].line;
    int cc = cur->b[1].col;
    int nextstate = (cur->state+1)%4;
    switch(nextstate) {
        /*
             <#>{#}
             [0][c][2]
             {#}{#}[3]
        */
        case 0:
            if(!isempty(cl-1,cc-1) || !isempty(cl-1,cc) ||
              !isempty(cl+1,cc-1) || !isempty(cl+1,cc))
                return 0;
            break;

        /*
             {#}[0]<#>
             {#}[c]{#}
             [3][2]
        */
        case 1:
            if(!isempty(cl-1,cc-1) || !isempty(cl-1,cc+1) ||
              !isempty(cl,cc-1) || !isempty(cl,cc+1))
                return 0;
            break;

        /*
             [3]{#}{#}
             [2][c][0]
                {#}<#>
        */
        case 2:
            if(!isempty(cl-1,cc) || !isempty(cl-1,cc+1) ||
              !isempty(cl+1,cc) || !isempty(cl+1,cc+1))
                return 0;
            break;

        /*
                [2][3]
             {#}[c]{#}
             <#>[0]{#}
        */
        case 3:
            if(!isempty(cl,cc-1) || !isempty(cl,cc+1) ||
              !isempty(cl+1,cc-1) || !isempty(cl+1,cc+1))
                return 0;
            break;
    }
    cur->rotate();
    return 1;
}

int tetris_logic::rotate_l(void) {
    int cl = cur->b[1].line;
    int cc = cur->b[1].col;
    int nextstate = (cur->state+1)%4;
    switch(nextstate) {
        /*
             <#>{#}[3]
             [0][c][2]
                {#}{#}
        */
        case 0:
            if(!isempty(cl-1,cc-1) || !isempty(cl-1,cc) ||
              !isempty(cl+1,cc) || !isempty(cl+1,cc+1))
                return 0;
            break;

        /*
                [0]<#>
             {#}[c]{#}
             {#}[2][3]
        */
        case 1:
            if(!isempty(cl-1,cc+1) || !isempty(cl,cc+1) ||
              !isempty(cl,cc-1) || !isempty(cl+1,cc-1))
                return 0;
            break;

        /*
             {#}{#}
             [2][c][0]
             [3]{#}<#>
        */
        case 2:
            if(!isempty(cl-1,cc-1) || !isempty(cl-1,cc) ||
              !isempty(cl+1,cc) || !isempty(cl+1,cc+1))
                return 0;
            break;

        /*
             [3][2]{#}
             {#}[c]{#}
             <#>[0]
        */
        case 3:
            if(!isempty(cl-1,cc+1) || !isempty(cl,cc+1) ||
              !isempty(cl,cc-1) || !isempty(cl+1,cc-1))
                return 0;
            break;
    }
    cur->rotate();
    return 1;
}

int tetris_logic::rotate_s(void) {
    int cl = cur->b[1].line;
    int cc = cur->b[1].col;
    int nextstate = (cur->state+1)%4;
    switch(nextstate) {
        /*
             [3]{#}{#}
             [2][c]
             <#>[0]
        */
        case 0:
        case 2:
            if(!isempty(cl-1,cc) || !isempty(cl-1,cc+1) || !isempty(cl+1,cc-1))
                return 0;
            break;

        /*
             {#}
             {#}[c][0]
             [3][2]<#>
        */
        case 1:
        case 3:
            if(!isempty(cl-1,cc-1) || !isempty(cl,cc-1) || !isempty(cl+1,cc+1))
                return 0;
            break;
    }
    cur->rotate();
    return 1;
}

int tetris_logic::rotate_t(void) {
    int cl = cur->b[1].line;
    int cc = cur->b[1].col;
    int nextstate = (cur->state+1)%4;
    switch(nextstate) {
        /*
                [2]<#>
             {#}[c][3]
             <#>[0]<#>
        */
        case 0:
            if(!isempty(cl,cc-1) || !isempty(cl+1,cc-1) ||
              !isempty(cl-1,cc+1) || !isempty(cl+1,cc+1))
                return 0;
            break;

        /*
             <#>{#}
             [0][c][2]
             <#>[3]<#>
        */
        case 1:
            if(!isempty(cl-1,cc-1) || !isempty(cl-1,cc) ||
              !isempty(cl+1,cc-1) || !isempty(cl+1,cc+1))
                return 0;
            break;

        /*
             <#>[0]<#>
             [3][c]{#}
             <#>[2]
        */
        case 2:
            if(!isempty(cl-1,cc-1) || !isempty(cl+1,cc-1) ||
              !isempty(cl-1,cc+1) || !isempty(cl,cc+1))
                return 0;
            break;

        /*
             <#>[3]<#>
             [2][c][0]
                {#}<#>
        */
        case 3:
            if(!isempty(cl-1,cc-1) || !isempty(cl-1,cc+1) ||
              !isempty(cl+1,cc) || !isempty(cl+1,cc+1))
                return 0;
            break;
    }
    cur->rotate();
    return 1;
}

int tetris_logic::rotate_z(void) {
    int cl = cur->b[1].line;
    int cc = cur->b[1].col;
    int nextstate = (cur->state+1)%4;
    switch(nextstate) {
        /*
             {#}[0]<#>
             [2][c]{#}
             [3]
        */
        case 0:
        case 2:
            if(!isempty(cl-1,cc-1) || !isempty(cl-1,cc+1) || !isempty(cl,cc+1))
                return 0;
            break;

        /*
             <#>{#}
             [0][c]
             {#}[2][3]
        */
        case 1:
        case 3:
            if(!isempty(cl-1,cc-1) || !isempty(cl-1,cc) || !isempty(cl+1,cc-1))
                return 0;
            break;
    }
    cur->rotate();
    return 1;
}

bool tetris_logic::islinefull(int n) {
    for(int i=0; i<cols; ++i) {
        if(EMPTY == playgrd[n][i])
            return false;
    }
    return true;
}

int tetris_logic::clearline(int n) {
    if( islinefull(n) ) {
        for(int i=n; i>0; --i)
            memcpy(playgrd[i], playgrd[i-1], cols*sizeof(int));
        memset(playgrd[0], 0, cols*sizeof(char));
        return 1;
    }
    return 0;
}

int tetris_logic::clearlines(void) {
    int from, to;
    from = to = cur->b[0].line;
    for(int i=1; i<4; ++i) {
        if(cur->b[i].line < from)
            from = cur->b[i].line;
        if(cur->b[i].line > to)
            to = cur->b[i].line;
    }
    int count = 0;
    for(int i=from; i<=to; ++i) {
        if(1 == clearline(i))
            ++count;
    }
    return count;
}

void tetris_logic::next(void) {
    fillcur(nexttyp, nextstate);
    nexttyp = rand()%7 + 2;
    nextstate = rand()%4;
    fillnext();
}

void tetris_logic::fillnext(void) {
    static int patterns[7][4][16] = {
        // i
        {
            {0,0,2,0,
             0,0,2,0,
             0,0,2,0,
             0,0,2,0},
            {0,0,0,0,
             0,0,0,0,
             2,2,2,2,
             0,0,0,0},
            {0,0,2,0,
             0,0,2,0,
             0,0,2,0,
             0,0,2,0},
            {0,0,0,0,
             0,0,0,0,
             2,2,2,2,
             0,0,0,0},
        },

        // j
        {
            {0,0,0,0,
             0,0,3,0,
             0,0,3,0,
             0,3,3,0},
            {0,0,0,0,
             0,0,0,0,
             0,3,0,0,
             0,3,3,3},
            {0,0,0,0,
             0,3,3,0,
             0,3,0,0,
             0,3,0,0},
            {0,0,0,0,
             0,0,0,0,
             0,3,3,3,
             0,0,0,3},
        },

        // l
        {
            {0,0,0,0,
             0,4,0,0,
             0,4,0,0,
             0,4,4,0},
            {0,0,0,0,
             0,0,0,0,
             0,4,4,4,
             0,4,0,0},
            {0,0,0,0,
             0,4,4,0,
             0,0,4,0,
             0,0,4,0},
            {0,0,0,0,
             0,0,0,0,
             0,0,0,4,
             0,4,4,4},
        },

        // o
        {
            {0,0,0,0,
             0,0,0,0,
             0,5,5,0,
             0,5,5,0},
            {0,0,0,0,
             0,0,0,0,
             0,5,5,0,
             0,5,5,0},
            {0,0,0,0,
             0,0,0,0,
             0,5,5,0,
             0,5,5,0},
            {0,0,0,0,
             0,0,0,0,
             0,5,5,0,
             0,5,5,0},
        },

        // s
        {
            {0,0,0,0,
             0,0,0,0,
             0,0,6,6,
             0,6,6,0},
            {0,0,0,0,
             0,6,0,0,
             0,6,6,0,
             0,0,6,0},
            {0,0,0,0,
             0,0,0,0,
             0,0,6,6,
             0,6,6,0},
            {0,0,0,0,
             0,6,0,0,
             0,6,6,0,
             0,0,6,0},
        },

        // t
        {
            {0,0,0,0,
             0,0,0,0,
             0,7,7,7,
             0,0,7,0},
            {0,0,0,0,
             0,0,7,0,
             0,7,7,0,
             0,0,7,0},
            {0,0,0,0,
             0,0,0,0,
             0,0,7,0,
             0,7,7,7},
            {0,0,0,0,
             0,7,0,0,
             0,7,7,0,
             0,7,0,0},
        },

        // z
        {
            {0,0,0,0,
             0,8,8,0,
             0,0,8,8,
             0,0,0,0},
            {0,0,0,0,
             0,8,0,0,
             0,8,8,0,
             0,0,8,0},
            {0,0,0,0,
             0,8,8,0,
             0,0,8,8,
             0,0,0,0},
            {0,0,0,0,
             0,8,0,0,
             0,8,8,0,
             0,0,8,0}
        }
    };

    memcpy(nextgrd, patterns[nexttyp-2][nextstate], sizeof(nextgrd));
}

void tetris_logic::fillcur(char type, int state) {
    if(cur != NULL) {
        delete cur;
        cur = NULL;
    }

    switch(type) {
        case I:
            i++;
            cur = new block_i(cols/2);
            break;

        case J:
            j++;
            cur = new block_j(cols/2);
            break;

        case L:
            l++;
            cur = new block_l(cols/2);
            break;

        case O:
            o++;
            cur = new block_o(cols/2);
            break;

        case S:
            s++;
            cur = new block_s(cols/2);
            break;

        case T:
            t++;
            cur = new block_t(cols/2);
            break;

        case Z:
            z++;
            cur = new block_z(cols/2);
            break;

        default:
            return;
    }

    cur->state = state==0 ? 3 : state-1;
    rotate();
    settlecur();
}

void tetris_logic::settlecur(void) {
    int line = cur->b[0].line;
    for(int i=1; i<4; ++i) {
        if(line < cur->b[i].line)
            line = cur->b[i].line;
    }
    for(; line<0; ++line)
        move_down();
}

void tetris_logic::stick(void) {
    for(int i=0; i<4; ++i) {
        if(cur->b[i].line < 0)
            continue;
        playgrd[cur->b[i].line][cur->b[i].col] = get_type(cur->type);
    }
}

bool tetris_logic::isgameover(void) {
    for(int i=0; i<cols; ++i) {
        if(playgrd[0][i] != EMPTY)
            return true;
    }
    return false;
}

void tetris_logic::gameover(void) {
}

void tetris_logic::onclearline(int n) {
    if(n<=0 || n>4)
        return;
    switch(n) {
        case 1:
            one++;
            break;
        case 2:
            two++;
            break;
        case 3:
            three++;
            break;
        case 4:
            four++;
            break;
    }
    score += scores[n-1];

    if(level<MAXLEVEL && score>levels[level])
        level++;
}

bool tetris_logic::isempty(int line, int col) {
    return (col>=0&&col<cols) && (line<0 || (line<lines&&playgrd[line][col]==EMPTY));
}

tetris_logic::TYPE tetris_logic::get_type(char c) {
    tetris_logic::TYPE ret = tetris_logic::EMPTY;
    switch( c ) {
        case 'E':
            ret = tetris_logic::EMPTY;
            break;
        case 'A':
            ret = tetris_logic::ACTIVE;
            break;
        case 'I':
            ret = tetris_logic::I;
            break;
        case 'J':
            ret = tetris_logic::J;
            break;
        case 'L':
            ret = tetris_logic::L;
            break;
        case 'O':
            ret = tetris_logic::O;
            break;
        case 'S':
            ret = tetris_logic::S;
            break;
        case 'T':
            ret = tetris_logic::T;
            break;
        case 'Z':
            ret = tetris_logic::Z;
            break;
    }
    return ret;
}

