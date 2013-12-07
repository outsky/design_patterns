#ifndef BLOCK_T_H
#define BLOCK_T_H

#include "block.h"

class block_t : public block {
    public:
        block_t(int col);
        void rotate();
};

#endif
