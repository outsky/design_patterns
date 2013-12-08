#include "block_z.h"

block_z::block_z(int col) : block('Z') {
    b[0].line=-2; b[0].col=col-1;    //
    b[1].line=-2; b[1].col=col;      //
    b[2].line=-1; b[2].col=col;      //01
    b[3].line=-1; b[3].col=col+1;    // 23
}

void block_z::rotate() {
    int cl = b[1].line;
    int cc = b[1].col;
    int next = (state+1)%4;
    switch(next) {
        /*
             {#}[0]<#>
             [2][c]{#}
             [3]
        */
        case 0:
        case 2:
            b[0].col = cc-1;
            b[2].col = cc;
            b[3].col = cc+1;
            b[0].line = cl;
            b[2].line = b[3].line = cl+1;
            break;

        /*
             <#>{#}
             [0][c]
             {#}[2][3]
        */
        case 1:
        case 3:
            b[0].col = cc;
            b[2].col = b[3].col = cc-1;
            b[0].line = cl-1;
            b[2].line = cl;
            b[3].line = cl+1;
            break;
    }
    state = next;
}
