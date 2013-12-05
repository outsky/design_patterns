#ifndef STRALG_LOWER_H
#define STRALG_LOWER_H

#include "stralg.h"

class stralg_lower : public stralg {
    public:
        void process(char* str);
};

void stralg_lower::process(char* str) {
    while(*str != '\0') {
        *str = tolower(*str);
        ++str;
    }
}

#endif
