#include <iostream>
#include <stdlib.h>

#include "operator.h"
#include "op_factory.h"

static op_factory::OPERATOR get_op(char c);
int main(int argc, char* argv[]) {
    if(argc != 4) {
        std::cout << "[x] USAGE: " << argv[0] << " num1 op(+,-,\\*,/) num2" << std::endl;
        return -1;
    }

    int num1 = atoi(argv[1]);
    char op = *argv[2];
    int num2 = atoi(argv[3]);

    op_base* op_ins = op_factory::make_operator(get_op(op));
    if(op_ins == NULL) {
        std::cout << "[x] Unknown operator " << op << std::endl;
        return -1;
    }

    int ret = op_ins->calc(num1, num2);
    std::cout << "[calc] " << num1 << op << num2 << " = " << ret << std::endl;

    return 0;
}

op_factory::OPERATOR get_op(char c) {
    op_factory::OPERATOR ret = op_factory::MAX;
    switch( c ) {
        case '+':
            ret = op_factory::ADD;
            break;
        case '-':
            ret = op_factory::SUB;
            break;
        case '*':
            ret = op_factory::MUL;
            break;
        case '/':
            ret = op_factory::DIV;
            break;
    }
    return ret;
}

