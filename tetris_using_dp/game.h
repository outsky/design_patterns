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

    private:
        static game* ins;
        tetris_logic* logic;
};

#endif
