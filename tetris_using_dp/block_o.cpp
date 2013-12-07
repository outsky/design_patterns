#include "block_o.h"

block_o::block_o(int col) : block('O') {
    b[0].line=-2; b[0].col=col;      //
    b[1].line=-2; b[1].col=col+1;    //
    b[2].line=-1; b[2].col=col;      // 01
    b[3].line=-1; b[3].col=col+1;    // 23
}

void block_o::rotate() {
}
