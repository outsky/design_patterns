#include "reactor.h"
#include <string.h>

reactor::ins = NULL;

reactor::reactor() {
    memset(&events, 0, sizeof(event_set));
}

reactor* reactor::instance() {
    if(ins == NULL)
        ins = new reactor();
    return ins;
}

bool reactor::register_handler(int event, event_handler* handler) {
    handler_t fd = handler->handle;
    events.data[fd].event = event;
    events.data[fd].handle = fd;
    events.data[fd].handler = handler->handler;

    return true;
}

void reactor::remove_handler(event_handler* handler) {

}

void reactor::handle_events() {

}
