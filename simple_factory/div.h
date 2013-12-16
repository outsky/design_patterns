#ifndef DIV_H
#define DIV_H

#include "operator.h"

class op_div : public op_base {
    public:
        int calc(int a, int b);
};

int op_div::calc(int a, int b) {
    if(b == 0)
        return 0;
    return a/b;
}

#endif
