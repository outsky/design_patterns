#ifndef EVENT_H
#define EVENT_H

#include <map>

#include "event_handler.h"

class event_set {
    public:
        struct unit {
            int event;
            event_handler* handler;
        };

        std::map<int, struct unit> data;
};

#endif
