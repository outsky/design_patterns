#ifndef STRALG_TRIM_H
#define STRALG_TRIM_H

#include "stralg.h"

class stralg_trim : public stralg {
    public:
        void process(char* str);
};

void stralg_trim::process(char* str) {
    while(*str != '\0') {
        if(*str == ' ')
            memmove(str, str+1, strlen(str));
        else
            ++str;
    }
}

#endif
