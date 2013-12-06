#include <stdlib.h>
#include <string.h>

#include "tetris_logic.h"

#define MAXLEVEL 11
int levels[MAXLEVEL] = {25, 50, 100, 180, 280, 400, 550, 700, 900, 1200, 1500};
static int scores[4] = {1, 5, 10, 20};
int speeds[MAXLEVEL] = {1000, 950, 900, 850, 800, 750, 700, 650, 600, 550, 500};

tetris_logic* tetris_logic::ins = NULL;

tetris_logic::tetris_logic() {
    pthread_mutex_init(&mut, NULL);
    pthread_cond_init(&cond, NULL);

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

int tetris_logic::can_move_down() {
    int i;
    for(i=0; i<4; ++i) {
        struct pos* c = &cur[i];
        if(c->line+1>=lines || (c->line+1>=0 && playgrd[c->line+1][c->col]!=0))
            return 0;
    }
    return 1;
}

int tetris_logic::can_move_left() {
    int i;
    for(i=0; i<4; ++i) {
        struct pos* c = &cur[i];
        if(c->col-1<0 || (c->line>=0 && playgrd[c->line][c->col-1]!=0))
            return 0;
    }
    return 1;
}

int tetris_logic::can_move_right() {
    int i;
    for(i=0; i<4; ++i) {
        struct pos* c = &cur[i];
        if(c->col+1>=cols || (c->line>=0 && playgrd[c->line][c->col+1]!=0))
            return 0;
    }
    return 1;
}

int tetris_logic::drop_down(void) {
    while(1 == move_down())
        ;
    return 0;
}

int tetris_logic::move_down() {
    if(1 != can_move_down()) {
        stick();
        if(1 == isgameover()) {
            gameover();
            return 0;
        }
        int ln = clearlines();
        onclearline(ln);
        next();
        return 0;
    }

    pthread_mutex_lock(&mut);
    int i;
    for(i=0; i<4; ++i)
        cur[i].line++;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mut);

    return 1;
}

int tetris_logic::move_right() {
    if(1 != can_move_right())
        return 0;

    pthread_mutex_lock(&mut);
    int i;
    for(i=0; i<4; ++i)
        cur[i].col++;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mut);

    return 1;
}

int tetris_logic::move_left() {
    if(1 != can_move_left())
        return 0;

    pthread_mutex_lock(&mut);
    int i;
    for(i=0; i<4; ++i)
        cur[i].col--;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mut);

    return 1;
}

int tetris_logic::rotate() {
    pthread_mutex_lock(&mut);
    int ret = 0;
    switch(curtype) {
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
    struct pos* c = cur;
    int cl = c[1].line;
    int cc = c[1].col;
    int nextstate = (curstate+1)%4;
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
            cur[0].col = cur[2].col = cur[3].col = cc;
            cur[0].line = cl-1;
            cur[2].line = cl+1;
            cur[3].line = cl+2;
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
            cur[0].line = cur[2].line = cur[3].line = cl;
            cur[0].col = cc+1;
            cur[2].col = cc-1;
            cur[3].col = cc-2;
            break;
    }
    curstate = nextstate;
    return 1;
}

int tetris_logic::rotate_j(void) {
    struct pos* c = cur;
    int cl = cur[1].line;
    int cc = cur[1].col;
    int nextstate = (curstate+1)%4;
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
            cur[0].col = cur[2].col = cc;
            cur[3].col = cc-1;
            cur[0].line = cl-1;
            cur[2].line = cur[3].line = cl+1;
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
            cur[0].line = cur[2].line = cl;
            cur[3].line = cl-1;
            cur[0].col = cc+1;
            cur[2].col = cur[3].col = cc-1;
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
            cur[0].col = cur[2].col = cc;
            cur[3].col = cc+1;
            cur[0].line = cl+1;
            cur[2].line = cur[3].line = cl-1;
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
            cur[0].line = cur[2].line = cl;
            cur[3].line = cl+1;
            cur[0].col = cc-1;
            cur[2].col = cur[3].col = cc+1;
            break;
    }
    curstate = nextstate;
    return 1;
}

int tetris_logic::rotate_l(void) {
    int cl = cur[1].line;
    int cc = cur[1].col;
    int nextstate = (curstate+1)%4;
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
            cur[0].col = cur[2].col = cc;
            cur[3].col = cc+1;
            cur[0].line = cl-1;
            cur[2].line = cur[3].line = cl+1;
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
            cur[0].line = cur[2].line = cl;
            cur[3].line = cl+1;
            cur[0].col = cc+1;
            cur[2].col = cur[3].col = cc-1;
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
            cur[0].col = cur[2].col = cc;
            cur[3].col = cc-1;
            cur[0].line = cl+1;
            cur[2].line = cur[3].line = cl-1;
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
            cur[0].line = cur[2].line = cl;
            cur[3].line = cl-1;
            cur[0].col = cc-1;
            cur[2].col = cur[3].col = cc+1;
            break;
    }
    curstate = nextstate;
    return 1;
}

int tetris_logic::rotate_s(void) {
    int cl = cur[1].line;
    int cc = cur[1].col;
    int nextstate = (curstate+1)%4;
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
            cur[0].col = cc+1;
            cur[2].col = cc;
            cur[3].col = cc-1;
            cur[0].line = cl;
            cur[2].line = cur[3].line = cl+1;
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
            cur[0].col = cc;
            cur[2].col = cur[3].col = cc-1;
            cur[0].line = cl+1;
            cur[2].line = cl;
            cur[3].line = cl-1;
            break;
    }
    curstate = nextstate;
    return 1;
}

int tetris_logic::rotate_t(void) {
    int cl = cur[1].line;
    int cc = cur[1].col;
    int nextstate = (curstate+1)%4;
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
            cur[0].col = cc-1;
            cur[2].col = cc+1;
            cur[3].col = cc;
            cur[0].line = cur[2].line = cl;
            cur[3].line = cl+1;
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
            cur[0].col = cur[2].col = cc;
            cur[3].col = cc-1;
            cur[0].line = cl-1;
            cur[2].line = cl+1;
            cur[3].line = cl;
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
            cur[0].col = cc+1;
            cur[2].col = cc-1;
            cur[3].col = cc;
            cur[0].line = cur[2].line = cl;
            cur[3].line = cl-1;
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
            cur[0].col = cur[2].col = cc;
            cur[3].col = cc+1;
            cur[0].line = cl+1;
            cur[2].line = cl-1;
            cur[3].line = cl;
            break;
    }
    curstate = nextstate;
    return 1;
}

int tetris_logic::rotate_z(void) {
    int cl = cur[1].line;
    int cc = cur[1].col;
    int nextstate = (curstate+1)%4;
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
            cur[0].col = cc-1;
            cur[2].col = cc;
            cur[3].col = cc+1;
            cur[0].line = cl;
            cur[2].line = cur[3].line = cl+1;
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
            cur[0].col = cc;
            cur[2].col = cur[3].col = cc-1;
            cur[0].line = cl-1;
            cur[2].line = cl;
            cur[3].line = cl+1;
            break;
    }
    curstate = nextstate;
    return 1;
}

int tetris_logic::islinefull(int n) {
    int i;
    for(i=0; i<cols; ++i) {
        if(0 == playgrd[n][i])
            return 0;
    }
    return 1;
}

int tetris_logic::clearline(int n) {
    if(1 == islinefull(n)) {
        int i;
        for(i=n; i>0; --i)
            memcpy(playgrd[i], playgrd[i-1], cols*sizeof(int));
        memset(playgrd[0], 0, cols*sizeof(int));
        return 1;
    }
    return 0;
}

int tetris_logic::clearlines(void) {
    int from, to;
    from = to = cur[0].line;
    int i;
    for(i=1; i<4; ++i) {
        if(cur[i].line < from)
            from = cur[i].line;
        if(cur[i].line > to)
            to = cur[i].line;
    }
    int count = 0;
    for(i=from; i<=to; ++i) {
        if(1 == clearline(i))
            ++count;
    }
    return count;
}

void tetris_logic::next(void) {
    static int i=0;
    curtype = nexttyp;
    curstate = nextstate;
    nexttyp = rand()%7 + 2;
    nextstate = rand()%4;
    fillnext();
    fillcur();
}

void tetris_logic::fillnext(void) {
    static int pattern[7][4][16] = {
        { // I
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
             0,0,0,0}
        },{ // J
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
             0,0,0,3}
        },{ // L
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
             0,4,4,4}
        },{ // O
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
             0,5,5,0}
        },{ // S
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
             0,0,6,0}
        },{ // T
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
             0,7,0,0}
        },{ // Z
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

    if(nexttyp>Z || nexttyp<I || nextstate<0 || nextstate>3)
        return;

    memcpy(nextgrd, pattern[nexttyp-2][nextstate], sizeof(nextgrd));
}

void tetris_logic::fillcur(void) {
    memset(cur, 0, sizeof(cur));
    switch(curtype) {
        case I:
            i++;
            cur[0].line=-4; cur[0].col=cols/2;      // 0
            cur[1].line=-3; cur[1].col=cols/2;      // 1
            cur[2].line=-2; cur[2].col=cols/2;      // 2
            cur[3].line=-1; cur[3].col=cols/2;      // 3
            break;

        case J:
            j++;
            cur[0].line=-3; cur[0].col=cols/2;      //
            cur[1].line=-2; cur[1].col=cols/2;      // 0
            cur[2].line=-1; cur[2].col=cols/2;      // 1
            cur[3].line=-1; cur[3].col=cols/2-1;    //32
            break;

        case L:
            l++;
            cur[0].line=-3; cur[0].col=cols/2;      //
            cur[1].line=-2; cur[1].col=cols/2;      // 0
            cur[2].line=-1; cur[2].col=cols/2;      // 1
            cur[3].line=-1; cur[3].col=cols/2+1;    // 23
            break;

        case O:
            o++;
            cur[0].line=-2; cur[0].col=cols/2;      //
            cur[1].line=-2; cur[1].col=cols/2+1;    //
            cur[2].line=-1; cur[2].col=cols/2;      // 01
            cur[3].line=-1; cur[3].col=cols/2+1;    // 23
            break;

        case S:
            s++;
            cur[0].line=-2; cur[0].col=cols/2+1;    //
            cur[1].line=-2; cur[1].col=cols/2;      //
            cur[2].line=-1; cur[2].col=cols/2;      // 10
            cur[3].line=-1; cur[3].col=cols/2-1;    //32
            break;

        case T:
            t++;
            cur[0].line=-2; cur[0].col=cols/2-1;    //
            cur[1].line=-2; cur[1].col=cols/2;      //
            cur[2].line=-2; cur[2].col=cols/2+1;    //012
            cur[3].line=-1; cur[3].col=cols/2;      // 3
            break;

        case Z:
            z++;
            cur[0].line=-2; cur[0].col=cols/2-1;    //
            cur[1].line=-2; cur[1].col=cols/2;      //
            cur[2].line=-1; cur[2].col=cols/2;      //01
            cur[3].line=-1; cur[3].col=cols/2+1;    // 23
            break;

        default:
            return;
    }

    curstate = curstate==0 ? 3 : curstate-1;
    rotate();
    settlecur();
}

void tetris_logic::settlecur(void) {
    int line = cur[0].line;
    int i;
    for(i=1; i<4; ++i) {
        if(line < cur[i].line)
            line = cur[i].line;
    }
    for(; line<0; ++line)
        move_down();
}

void tetris_logic::stick(void) {
    int i;
    for(i=0; i<4; ++i) {
        if(cur[i].line < 0)
            continue;
        playgrd[cur[i].line][cur[i].col] = curtype;
    }
}

int tetris_logic::isgameover(void) {
    int i;
    for(i=0; i<cols; ++i) {
        if(playgrd[0][i] != 0)
            return 1;
    }
    return 0;
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
