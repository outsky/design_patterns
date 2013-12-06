#ifndef GAME_H
#define GAME_H

#include "tetris_logic.h"

class game {
    public:
        enum STATE {
            READY = 0,
            PLAY,
            PAUSE,
            OVER
        };

    public:
        static game* instance();
        void draw();

        void move_down();
        void move_left();
        void move_right();
        void drop_down();
        void rotate();

        void adjust_position();

    private:
        game();
        void draw_playgrd(void);
        void draw_preview(void);
        void draw_linerecord(void);
        void draw_blockrecord(void);
        void draw_status(void);
        void draw_pad(int bc, int n);
        void draw_block(int preview, int n);
        void draw_cur(void);

    public:
        int PGRD_LEFT;
        int PGRD_TOP;
        int PRV_LEFT;
        int PRV_TOP;
        int ST_LEFT;
        int ST_TOP;
        int LR_LEFT;
        int LR_TOP;
        int BR_LEFT;
        int BR_TOP;

    private:
        static game* ins;
        tetris_logic* logic;
};

#endif
