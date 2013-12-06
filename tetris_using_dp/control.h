#ifndef CONTROL_H
#define CONTROL_H

#include <termios.h>

class control {
    public:
        static control* instance();

        void cursor_to(int line, int col);
        void cursor_up(int n);
        void cursor_down(int n);
        void cursor_left(int n);
        void cursor_right(int n);

        void cursor_hide();
        void cursor_show();
        void cursor_save();
        void cursor_restore();

        // for buff problems
        void prepare_input();
        void restore_input();

    private:
        control();

    private:
        static control* ins;
        struct termios tm_org;
};

#endif
