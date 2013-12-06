#include "timer.h"

timer* timer::ins = NULL;

timer::timer() {
    reset();
}

timer* timer::instance() {
    if(ins == NULL)
        ins = new timer();
    return ins;
}

void timer::update() {
    struct timeval now;
    gettimeofday(&now, NULL);
    interval = (now.tv_sec-last.tv_sec)*1000 + (now.tv_usec-last.tv_usec)/1000;
}

int timer::get_interval() {
    return interval;
}

void timer::reset(void) {
    gettimeofday(&last, NULL);
    interval = 0;
}
