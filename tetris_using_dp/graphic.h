#ifndef GRAPHIC_H
#define GRAPHIC_H

class graphic {
    public:
        enum COLOR_FRONT {
            F_BLACK = 30,
            F_RED = 31,
            F_GREEN = 32,
            F_YELLOW = 33,
            F_BLUE = 34,
            F_MAGENTA = 35,
            F_CYAN = 36,
            F_WHITE = 37
        };
        enum COLOR_BACK {
            B_BLACK = 40,
            B_RED = 41,
            B_GREEN = 42,
            B_YELLOW = 43,
            B_BLUE = 44,
            B_MAGENTA = 45,
            B_CYAN = 46,
            B_WHITE = 47
        };
        enum TEXT_ATTR {
            T_BOLD = 1,
            T_UNDERSCORE = 4,
            T_BLINK = 5,
            T_REVERSE = 7
        };

    public:
        static void setattr(int i);
        static void setcolor(int f, int b);
        static void restore();

        static void erase_display();
        static void erase_line();
};

#endif
