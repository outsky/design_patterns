#include <stdio.h>
#include "graphic.h"

void graphic::setattr(int i) {
    printf("\033[%dm", i);
}

void graphic::setcolor(int f, int b) {
    printf("\033[%d;%dm", f, b);
}

void graphic::restore() {
    setattr(0);
}

void graphic::erase_display() {
    printf("\033[2J");
}

void graphic::erase_line() {
    printf("\033[K");
}
