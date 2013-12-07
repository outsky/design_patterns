#ifndef BLOCK_O_H
#define BLOCK_O_H

#include "block.h"

class block_o : public block {
    public:
        block_o(int col);
        void rotate();
};

#endif
