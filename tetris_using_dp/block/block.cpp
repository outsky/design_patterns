#include "block.h"

block::block(char t) : type(t) {
}

void block::move_down() {
    for(int i=0; i<4; ++i)
        ++b[i].line;
}

void block::move_right() {
    for(int i=0; i<4; ++i)
        ++b[i].col;
}

void block::move_left() {
    for(int i=0; i<4; ++i)
        --b[i].col;
}
