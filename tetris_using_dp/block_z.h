#ifndef BLOCK_Z_H
#define BLOCK_Z_H

#include "block.h"

class block_z : public block {
    public:
        block_z(int col);
        void rotate();
};

#endif
