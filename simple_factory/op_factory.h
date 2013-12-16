#ifndef OP_FACTORY_H
#define OP_FACTORY_H

#include "operator.h"
#include "add.h"
#include "sub.h"
#include "mul.h"
#include "div.h"

class op_factory {
    public:
        enum OPERATOR {
            ADD = 1,
            SUB,
            MUL,
            DIV,
            MAX
        };

        static op_base* make_operator(OPERATOR type);
};

op_base* op_factory::make_operator(OPERATOR type) {
    op_base* ret = NULL;
    switch( type ) {
        case ADD:
            ret = new op_add();
            break;
        case SUB:
            ret = new op_sub();
            break;
        case MUL:
            ret = new op_mul();
            break;
        case DIV:
            ret = new op_div();
            break;
    }
    return ret;
}

#endif
