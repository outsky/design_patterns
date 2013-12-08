#ifndef BLOCK_S_H
#define BLOCK_S_H

#include "block.h"

class block_s : public block {
    public:
        block_s(int col);
        void rotate();
};

#endif
