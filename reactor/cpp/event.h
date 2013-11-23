#ifndef EVENT_H
#define EVENT_H

typedef int handle_t;

class event_handler;
class event_set {
    public:
        struct unit {
            int event;
            handle_t handle;
            event_handler* handler;
        };

        unit data[FD_SETSIZE];
};

#endif
