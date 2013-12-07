#include "block_i.h"

block_i::block_i(int col) : block('I') {
    b[0].line=-4; b[0].col=col;      // 0
    b[1].line=-3; b[1].col=col;      // 1
    b[2].line=-2; b[2].col=col;      // 2
    b[3].line=-1; b[3].col=col;      // 3
}

void block_i::rotate() {
    int cl = b[1].line;
    int cc = b[1].col;
    int next = (state+1)%4;
    switch(next) {
        /*
            <#><#>{#}
            [3][2][c][0]
                  {#}<#>
                  {#}
        */
        case 0:
        case 2:
            b[0].col = b[2].col = b[3].col = cc;
            b[0].line = cl-1;
            b[2].line = cl+1;
            b[3].line = cl+2;
            break;

        /*
                [0]<#>
          {#}{#}[c]{#}
             <#>[2]
             <#>[3]
        */
        case 1:
        case 3:
            b[0].line = b[2].line = b[3].line = cl;
            b[0].col = cc+1;
            b[2].col = cc-1;
            b[3].col = cc-2;
            break;
    }
    state = next;
}
