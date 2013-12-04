#ifndef SUB_H
#define SUB_H

#include "operator.h"

class op_sub : public op_base {
    public:
        int calc(int a, int b);
};

int op_sub::calc(int a, int b) {
    return a-b;
}

#endif
