#ifndef BLOCK_I_H
#define BLOCK_I_H

#include "block.h"

class block_i : public block {
    public:
        block_i(int col);
        void rotate();
};

#endif
