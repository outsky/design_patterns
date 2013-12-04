#ifndef MUL_H
#define MUL_H

#include "operator.h"

class op_mul : public op_base {
    public:
        int calc(int a, int b);
};

int op_mul::calc(int a, int b) {
    return a*b;
}

#endif
