#ifndef GAME_H
#define GAME_H

#include "timer.h"
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

        void move_down();
        void move_left();
        void move_right();
        void drop_down();
        void rotate();

        void draw();
        void adjust_position();

        void prepare();
        void run();
        void quit();

    private:
        game();

        static void sigint(int);
        static void on_quit(void);
        void quit_handler_init();

        static void* trd_draw(void*);
        static void* trd_timer(void*);

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
        timer* tm;
};

#endif
