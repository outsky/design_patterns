#ifndef TIMER_H
#define TIMER_H

#include <stdlib.h>
#include <sys/time.h>

class timer {
    public:
        static timer* instance();

        void update();
        int get_interval();
        void reset(void);

    private:
        timer();

    private:
        static timer* ins;
        struct timeval last;
        int interval; // ms
};

#endif
