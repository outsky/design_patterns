#ifndef STR_PROCESSOR_H
#define STR_PROCESSOR_H

#include "stralg.h"
#include "stralg_lower.h"
#include "stralg_upper.h"
#include "stralg_trim.h"

class str_processor {
    public:
        enum ALGTYPE {
            UPPER,
            LOWER,
            TRIM,
        };
        str_processor(ALGTYPE type);
        void process(char* str);

    private:
        stralg* alg;
};

str_processor::str_processor(str_processor::ALGTYPE type) {
    alg = NULL;
    switch( type ) {
        case UPPER:
            alg = new stralg_upper();
            break;
        case LOWER:
            alg = new stralg_lower();
            break;
        case TRIM:
            alg = new stralg_trim();
            break;
    }
}

void str_processor::process(char* str) {
    if(alg != NULL)
        alg->process(str);
}

#endif
