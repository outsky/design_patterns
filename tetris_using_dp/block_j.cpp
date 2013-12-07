#include "block_j.h"

block_j::block_j(int col) : block('J') {
    b[0].line=-3; b[0].col=col;      //
    b[1].line=-2; b[1].col=col;      // 0
    b[2].line=-1; b[2].col=col;      // 1
    b[3].line=-1; b[3].col=col-1;    //32
}

void block_j::rotate() {
    int cl = b[1].line;
    int cc = b[1].col;
    int next = (state+1)%4;
    switch(next) {
        /*
             <#>{#}
             [0][c][2]
             {#}{#}[3]
        */
        case 0:
            b[0].col = b[2].col = cc;
            b[3].col = cc-1;
            b[0].line = cl-1;
            b[2].line = b[3].line = cl+1;
            break;

        /*
             {#}[0]<#>
             {#}[c]{#}
             [3][2]
        */
        case 1:
            b[0].line = b[2].line = cl;
            b[3].line = cl-1;
            b[0].col = cc+1;
            b[2].col = b[3].col = cc-1;
            break;

        /*
             [3]{#}{#}
             [2][c][0]
                {#}<#>
        */
        case 2:
            b[0].col = b[2].col = cc;
            b[3].col = cc+1;
            b[0].line = cl+1;
            b[2].line = b[3].line = cl-1;
            break;

        /*
                [2][3]
             {#}[c]{#}
             <#>[0]{#}
        */
        case 3:
            b[0].line = b[2].line = cl;
            b[3].line = cl+1;
            b[0].col = cc-1;
            b[2].col = b[3].col = cc+1;
            break;
    }
    state = next;
}
