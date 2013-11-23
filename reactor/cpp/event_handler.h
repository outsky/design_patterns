#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

class event_handler {
    public:
        void handle_read() = 0;
        void handle_write() = 0;
        handle_t get_handle() = 0;
};

#endif
