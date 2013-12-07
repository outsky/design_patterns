#ifndef TETRIS_LOGIC_H
#define TETRIS_LOGIC_H

extern int speeds[];
extern int levels[];

#include <map>
#include "block.h"

class tetris_logic {
    public:
        enum TYPE {
            EMPTY = 0, ACTIVE,
            I, J, L, O, S, T, Z
        };

    public:
        static tetris_logic* instance();
        void destroy();

        int move_down();
        int move_right();
        int move_left();
        int rotate();
        int drop_down(void);

        static TYPE get_type(char c);

    private:
        tetris_logic();

        bool can_move_down();
        bool can_move_left();
        bool can_move_right();

        void next(void);
        void fillnext(void);
        void fillcur(char type, int state);
        void settlecur(void);
        void stick(void);
        void gameover(void);
        bool isgameover(void);

        bool islinefull(int n);
        int clearline(int n);
        int clearlines(void);
        void onclearline(int n);

        int rotate_i(void);
        int rotate_j(void);
        int rotate_l(void);
        int rotate_s(void);
        int rotate_t(void);
        int rotate_z(void);

        bool isempty(int line, int col);

    public:
        static const int lines = 20;
        static const int cols = 10;
        pthread_cond_t cond;
        pthread_mutex_t mut;

        block* cur;
        TYPE playgrd[lines][cols];
        TYPE nextgrd[4][4];
        int score;
        int level;
        int one,two,three,four;
        int i,j,l,o,s,t,z;

    private:
        static tetris_logic* ins;
        int nexttyp;
        int nextstate;
};

#endif
