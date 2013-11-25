#ifndef REACTOR_H
#define REACTOR_H

#include "event.h"
#include "event_handler.h"

class reactor {
    public:
        static reactor* instance();

        bool register_handler(event_t event, event_handler* handler);
        void remove_handler(handle_t handle);
        void handle_events();

    private:
        reactor();
        static reactor* ins;
        event_set events;

        int efd;
};

#endif
