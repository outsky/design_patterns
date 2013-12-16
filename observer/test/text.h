#ifndef TEXT_H
#define TEXT_H

#include <iostream>
#include "../observer.h"

class text : public observer {
    public:
        text(hp* h);

        virtual void update(void);

    private:
        void dead(void);
        void low(void);
        void high(void);

        hp* health;
};

text::text(hp* h) : health(h), observer(h) {
}

void text::update(void) {
    int max = health->get_max();
    int cur = health->get_health();

    float per = (float)cur/max;
    if(per <= 0.0f) {
        dead();
    } else if(per <= 0.5f) {
        low();
    } else {
        high();
    }
}

void text::dead(void) {
    std::cout << "[text] dead" << std::endl;
}

void text::low(void) {
    std::cout << "[text] low" << std::endl;
}

void text::high(void) {
    std::cout << "[text] high" << std::endl;
}

#endif
