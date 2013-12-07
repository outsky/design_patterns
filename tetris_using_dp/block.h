#ifndef BLOCK_H
#define BLOCK_H

class block {
    public:
        struct pos {
            int line;
            int col;
        };

    public:
        block(char t);

        void move_down();
        void move_right();
        void move_left();
        virtual void rotate() = 0;

    public:
        pos b[4];
        int state;
        const char type;
};

#endif
