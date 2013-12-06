#include <stdio.h>

#include "control.h"

control* control::ins = NULL;

control::control() {
}

control* control::instance() {
    if(ins == NULL)
        ins = new control();
    return ins;
}

void control::cursor_to(int line, int col) {
    printf("\033[%d;%dH", col, line);
}

void control::cursor_up(int n) {
    printf("\033[%dA", n);
}

void control::cursor_down(int n) {
    printf("\033[%dB", n);
}

void control::cursor_left(int n) {
    printf("\033[%dD", n);
}

void control::cursor_right(int n) {
    printf("\033[%dC", n);
}

void control::cursor_save() {
    printf("\033[s");
}

void control::cursor_restore() {
    printf("\033[u");
}

void control::cursor_hide() {
    printf("\033[?25l");
}

void control::cursor_show() {
    printf("\033[?25h");
}

void control::prepare_input() {
    struct termios tm;
    tcgetattr(0, &tm_org);
    tm = tm_org;
    tm.c_lflag &= (~ICANON);
    tm.c_lflag &= (~(ECHO|ECHOE|ECHOK|ECHONL));
    tm.c_cc[VTIME] = 0;
    tm.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &tm);

    cursor_hide();
}

void control::restore_input() {
    tcsetattr(0, TCSANOW, &tm_org);
    cursor_show();
}
