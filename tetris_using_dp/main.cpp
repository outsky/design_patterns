#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>

#include "timer.h"
#include "control.h"
#include "graphic.h"
#include "tetris_logic.h"
#include "game.h"

static void sigint(int);
static void quit(void);

static void* trd_draw(void*);
static void* trd_timer(void*);

int main() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = sigint;
    sigaction(SIGINT, &sa, NULL);
    atexit(quit);
    control::instance()->prepare_input();
    graphic::erase_display();

    srand(time(NULL));
    game::instance()->draw();

    pthread_mutex_init(&mut, NULL);
    pthread_cond_init(&cond, NULL);

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
            quit();
        else
            continue;
    }

    return 0;
}

static void quit(void) {
    graphic::restore();
    control::instance()->restore_input();
    graphic::erase_display();
    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&cond);
    printf("\n");
    exit(0);
}

static void* trd_draw(void* p) {
    for(;;) {
        if(0 == pthread_mutex_lock(&mut)) {
            pthread_cond_wait(&cond, &mut);
            game::instance()->draw();
            pthread_mutex_unlock(&mut);
        }
    }
}

static void* trd_timer(void* p) {
    timer_init();
    for(;;) {
        timer_update();
        if(timer_interval() >= speeds[tetris_logic::instance()->level]) {
            game::instance()->move_down();
            timer_reset();
        }
        usleep(50);
    }
}

static void sigint(int n) {
    quit();
    exit(0);
}
