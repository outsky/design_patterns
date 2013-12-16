#ifndef BAR_H
#define BAR_H

#include <iostream>
#include "../observer.h"

class bar : public observer {
    public:
        bar(hp* h);

        virtual void update(void);

    private:
        void draw(int cur, int max);
        hp* health;
};

bar::bar(hp* h) : health(h), observer(h) {
}

void bar::update(void) {
    int max = health->get_max();
    int cur = health->get_health();

    draw(cur, max);
}

void bar::draw(int cur, int max) {
    int m = max/10;
    int c = cur/10;
    for(int i=0; i<c; ++i)
        std::cout << "-";
    std::cout << std::endl;
}

#endif
