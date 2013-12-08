#include "block_s.h"

block_s::block_s(int col) : block('S') {
    b[0].line=-2; b[0].col=col+1;    //
    b[1].line=-2; b[1].col=col;      //
    b[2].line=-1; b[2].col=col;      // 10
    b[3].line=-1; b[3].col=col-1;    //32
}

void block_s::rotate() {
    int cl = b[1].line;
    int cc = b[1].col;
    int next = (state+1)%4;
    switch(next) {
        /*
             [3]{#}{#}
             [2][c]
             <#>[0]
        */
        case 0:
        case 2:
            b[0].col = cc+1;
            b[2].col = cc;
            b[3].col = cc-1;
            b[0].line = cl;
            b[2].line = b[3].line = cl+1;
            break;

        /*
             {#}
             {#}[c][0]
             [3][2]<#>
        */
        case 1:
        case 3:
            b[0].col = cc;
            b[2].col = b[3].col = cc-1;
            b[0].line = cl+1;
            b[2].line = cl;
            b[3].line = cl-1;
    }
    state = next;
}
