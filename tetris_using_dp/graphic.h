#ifndef GRAPHIC_H
#define GRAPHIC_H

class graphic {
    public:
        static void set_color(int color, int bg);
        static void reset_color();

        static void draw_text(const char* str, ...);
        static void draw_text(int color, int bg, const char* str, ...);
};

#endif
