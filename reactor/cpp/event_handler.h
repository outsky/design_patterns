#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "event.h"

class event_handler {
    public:
        virtual void handle_read() {}
        virtual void handle_write() {}
        virtual handle_t get_handle() = 0;
};

#endif
