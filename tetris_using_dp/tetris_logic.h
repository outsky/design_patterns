#ifndef TETRIS_LOGIC_H
#define TETRIS_LOGIC_H

#include <pthread.h>

class tetris_logic {
    public:
        enum TYPE {
            EMPTY = 0, ACTIVE,
            I, J, L, O, S, T, Z
        };
        struct pos {
            int line;
            int col;
        };

    public:
        static tetris_logic* instance();
        void game_init();

        int move_down();
        int move_right();
        int move_left();
        int rotate();
        int drop_down(void);

        int clearlines(void);

    private:
        tetris_logic();

        int can_move_down();
        int can_move_left();
        int can_move_right();

        void next(void);
        void fillnext(void);
        void fillcur(void);
        void settlecur(void);
        void stick(void);
        void gameover(void);
        int isgameover(void);

        void onclearline(int n);

        int rotate_i(void);
        int rotate_j(void);
        int rotate_l(void);
        int rotate_s(void);
        int rotate_t(void);
        int rotate_z(void);

        int islinefull(int n);
        int clearline(int n);
        bool isempty(int line, int col);

    public:
        static const int lines = 20;
        static const int cols = 10;
        int curtype;
        struct pos cur[4];
        int playgrd[lines][cols]; // 0(empty); 1(active); 2(I); 3(J); 4(L); 5(O); 6(S); 7(T); 8(Z)
        int nextgrd[4][4];
        int score;
        int level;
        int one,two,three,four;
        int i,j,l,o,s,t,z;

    private:
        static tetris_logic* ins;
        int curstate;
        int nexttyp;
        int nextstate;
};

extern int speeds[];
extern int levels[];

extern pthread_cond_t cond;
extern pthread_mutex_t mut;

#endif
