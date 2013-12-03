#ifndef HP_H
#define HP_H

#include "../subject.h"

class hp : public subject {
    public:
        hp(void) : health(0) {
        }

        void set_health(int n) {
            health = n>max ? max : (n<0 ? 0 : n);
            notify_observers();
        }

        int get_health(void) {
            return health;
        }

        int get_max(void) {
            return max;
        }

    private:
        static const int max = 100;
        int health;
};

#endif
