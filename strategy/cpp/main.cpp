#include <iostream>
#include <stdlib.h>
#include <readline/readline.h>

#include "str_processor.h"

const char* alg = "\n-----------------\n \
0) to uppercase\n \
1) to lowercase\n \
2) remove spaces\n \
*empty string to quit\n \
YOUR CHOICE:";

str_processor::ALGTYPE TYPE[] = {str_processor::UPPER, str_processor::LOWER, str_processor::TRIM};

int main(int argc, char* argv[]) {
    char* str = readline("inputs a string: ");
    while( true ) {
        char* choice = readline(alg);
        if(choice==NULL || choice[0]=='\0')
            break;

        int t = atoi(choice);
        free(choice);
        if(t<0 || t>3)
            break;

        str_processor sp(TYPE[t]);
        char* str_tmp = strdup(str);
        std::cout << "org: " << str << std::endl;
        sp.process(str_tmp);
        std::cout << "now: " << str_tmp << std::endl;
        free(str_tmp);
    }

    free(str);

    return 0;
}
