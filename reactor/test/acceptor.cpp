#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>

#include "acceptor.h"
#include "logger.h"
#include "../epoll_reactor.h"
#include "msg.h"

extern FILE* logfile;

accept_handler::accept_handler(int fd) : event_handler(fd) {
}

void accept_handler::handle_read() {
    int fd = -1;
    if((fd=accept(get_handle(), NULL, NULL)) == -1) {
        std::cout << "[!] accept: " << strerror(errno) << std::endl;
        return;
    }
    std::cout << "[?] accept new connection " << fd << std::endl;

    log_handler* lh = new log_handler(fd);
    epoll_reactor::instance()->register_handler(event_handler::READ, lh);
}

