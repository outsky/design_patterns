#ifndef EPOLL_REACTOR_H
#define EPOLL_REACTOR_H

#include "event_set.h"
#include "event_handler.h"

class epoll_reactor {
    public:
        static epoll_reactor* instance();

        bool register_handler(int event, event_handler* handler);
        void remove_handler(int handle);
        void handle_events();

    private:
        epoll_reactor();
        static epoll_reactor* ins;
        event_set events;

        int epoll_fd;
};

#endif
