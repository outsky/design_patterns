#ifndef STRALG_UPPER_H
#define STRALG_UPPER_H

#include "stralg.h"

class stralg_upper : public stralg {
    public:
        void process(char* str);
};

void stralg_upper::process(char* str) {
    while(*str != '\0') {
        *str = toupper(*str);
        ++str;
    }
}

#endif
