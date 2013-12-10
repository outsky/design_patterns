#include <stdio.h>
#include "ansi_escape.h"

void ansi_escape::cursor_to(int line, int col) {
    printf("\033[%d;%dH", col, line);
}

void ansi_escape::cursor_up(int n) {
    printf("\033[%dA", n);
}

void ansi_escape::cursor_down(int n) {
    printf("\033[%dB", n);
}

void ansi_escape::cursor_left(int n) {
    printf("\033[%dD", n);
}

void ansi_escape::cursor_right(int n) {
    printf("\033[%dC", n);
}

void ansi_escape::cursor_save() {
    printf("\033[s");
}

void ansi_escape::cursor_restore() {
    printf("\033[u");
}

void ansi_escape::cursor_hide() {
    printf("\033[?25l");
}

void ansi_escape::cursor_show() {
    printf("\033[?25h");
}

void ansi_escape::setattr(int i) {
    printf("\033[%dm", i);
}

void ansi_escape::setcolor(int f, int b) {
    printf("\033[%d;%dm", f, b);
}

void ansi_escape::erase_display() {
    printf("\033[2J");
}

void ansi_escape::erase_line() {
    printf("\033[K");
}
