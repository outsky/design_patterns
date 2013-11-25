#ifndef EVENT_H
#define EVENT_H

#include <map>

enum event_t {
    READ,
    WRITE,
};

typedef int handle_t;

class event_handler;
class event_set {
    public:
        struct unit {
            event_t event;
            handle_t handle;
            event_handler* handler;
        };

        std::map<handle_t, struct unit> data;
};

#endif
