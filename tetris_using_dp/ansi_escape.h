#ifndef ANSI_ESCAPE_H
#define ANSI_ESCAPE_H

class ansi_escape {
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
        static void cursor_to(int line, int col);
        static void cursor_up(int n);
        static void cursor_down(int n);
        static void cursor_left(int n);
        static void cursor_right(int n);

        static void cursor_hide();
        static void cursor_show();
        static void cursor_save();
        static void cursor_restore();

        static void setattr(int i);
        static void setcolor(int f, int b);

        static void erase_display();
        static void erase_line();
};

#endif
