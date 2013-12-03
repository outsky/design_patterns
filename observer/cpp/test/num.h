#ifndef NUM_H
#define NUM_H

#include <iostream>
#include "../observer.h"

class num : public observer {
    public:
        num(hp* h);

        virtual void update(void);

    private:
        hp* health;
};

num::num(hp* h) : health(h), observer(h) {
}

void num::update(void) {
    int max = health->get_max();
    int cur = health->get_health();

    float per = (float)cur/max;
    std::cout << "[NUM] " << cur << "/" << max << "(" << per*100.0f << "%)" << std::endl;
}

#endif
