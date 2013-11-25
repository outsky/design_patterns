#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include "../event_handler.h"

class accept_handler : public event_handler {
    public:
        accept_handler(int fd);

        void handle_read();
        int get_handle() { return handle; }

    private:
        int handle;
};

class log_handler : public event_handler {
    public:
        log_handler(int fd);

        void handle_read();
        void handle_write();
        int get_handle() { return handle; }

    private:
        int handle;
};

#endif
