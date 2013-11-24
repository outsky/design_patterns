#ifndef MSG_H
#define MSG_H

struct msg {
    enum t {
        info = 1,
        err = 2,
    };
    int size;
    int type;
    char name[10];
    char data[0];
};

#endif
