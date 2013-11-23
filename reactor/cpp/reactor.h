#ifndef REACTOR_H
#define REACTOR_H

#include "event_handler.h"
#include "event.h"

class reactor {
    public:
        static reactor* instance();

        bool register_handler(int event, event_handler* handler);
        void remove_handler(event_handler* handler);
        void handle_events();

    private:
        reactor();
        static reactor* ins;
        event_set events;
};

#endif
