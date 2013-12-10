#include <string.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <assert.h>

#include "epoll_reactor.h"

epoll_reactor* epoll_reactor::ins = NULL;

epoll_reactor::epoll_reactor() {
    if((epoll_fd=epoll_create1(0)) == -1)
        exit(-1);
}

epoll_reactor* epoll_reactor::instance() {
    if(ins == NULL)
        ins = new epoll_reactor();
    return ins;
}

bool epoll_reactor::register_handler(int event, event_handler* handler) {
    int fd = handler->get_handle();

    struct epoll_event ev;
    memset(&ev, 0, sizeof(struct epoll_event));
    ev.data.fd = fd;
    ev.events = (event&event_handler::WRITE)!=0 ? EPOLLOUT : 0;
    ev.events |= ((event&event_handler::READ)!=0 || (event&event_handler::CONNECT)!=0 ||
            (event&event_handler::DISCONNECT)!=0 || (event&event_handler::TIMEOUT)!=0) ? EPOLLIN : 0;
    if(events.data.find(fd) != events.data.end()) {
        if(epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &ev) == -1)
            return false;
        delete events.data[fd].handler;
    } else {
        if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1)
            return false;
    }

    events.data[fd].event = event;
    events.data[fd].handler = handler;

    return true;
}

void epoll_reactor::remove_handler(int handle) {
    if(epoll_ctl(epoll_fd, EPOLL_CTL_DEL, handle, NULL) == -1)
        return;
    delete events.data[handle].handler;
    events.data.erase(handle);
}

void epoll_reactor::handle_events() {
    struct epoll_event evs[1024];
    int n = epoll_wait(epoll_fd, evs, 1024, -1);
    for(int i=0; i<n; ++i) {
        // get handler
        int handle = evs[i].data.fd;
        std::map<int, event_set::unit>::const_iterator it = events.data.find(handle);
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
