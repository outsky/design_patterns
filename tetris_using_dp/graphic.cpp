#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "ansi_escape.h"
#include "graphic.h"

void graphic::set_color(int color, int bg) {
    ansi_escape::setcolor(color, bg);
}

void graphic::reset_color() {
    ansi_escape::setattr(0);
}

void graphic::draw_text(const char* str, ...) {
    char buf[64];
    va_list ap;
    va_start(ap, str);
    sprintf(buf, str, ap);
    va_end(ap);
    printf("%s", buf);
}

void graphic::draw_text(int color, int bg, const char* str, ...) {
    set_color(color, bg);

    char buf[64];
    va_list ap;
    va_start(ap, str);
    sprintf(buf, str, ap);
    va_end(ap);
    printf("%s", buf);

    reset_color();
}

