#include <string.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <assert.h>

#include "reactor.h"

reactor* reactor::ins = NULL;

reactor::reactor() {
    if((efd=epoll_create1(0)) == -1)
        exit(-1);
}

reactor* reactor::instance() {
    if(ins == NULL)
        ins = new reactor();
    return ins;
}

bool reactor::register_handler(event_t event, event_handler* handler) {
    handle_t fd = handler->get_handle();

    if(event!=READ && event!=WRITE)
        return false;

    struct epoll_event ev;
    memset(&ev, 0, sizeof(struct epoll_event));
    ev.data.fd = fd;
    ev.events = event==READ ? EPOLLIN : EPOLLOUT;
    if(events.data.find(fd) != events.data.end()) {
        if(epoll_ctl(efd, EPOLL_CTL_MOD, fd, &ev) == -1)
            return false;
        delete events.data[fd].handler;
    } else {
        if(epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev) == -1)
            return false;
    }

    events.data[fd].event = event;
    events.data[fd].handle = fd;
    events.data[fd].handler = handler;

    return true;
}

void reactor::remove_handler(handle_t handle) {
    if(epoll_ctl(efd, EPOLL_CTL_DEL, handle, NULL) == -1)
        return;
    delete events.data[handle].handler;
    events.data.erase(handle);
}

void reactor::handle_events() {
    struct epoll_event evs[1024];
    int n = epoll_wait(efd, evs, 1024, -1);
    for(int i=0; i<n; ++i) {
        // get handler
        handle_t handle = evs[i].data.fd;
        std::map<handle_t, event_set::unit>::const_iterator it = events.data.find(handle);
        assert(it != events.data.end());

        event_handler* handler = it->second.handler;
        assert(handler != NULL);

        // read event
        if((evs[i].events&EPOLLIN) != 0)
            handler->handle_read();

        // write event
        if((evs[i].events&EPOLLOUT) != 0)
            handler->handle_write();
    }
}
