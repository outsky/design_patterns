#ifndef ADD_H
#define ADD_H

#include "operator.h"

class op_add : public op_base{
    public:
        int calc(int a, int b);
};

int op_add::calc(int a, int b) {
    return a+b;
}

#endif
