#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>

#include "logger.h"
#include "../epoll_reactor.h"
#include "msg.h"

extern FILE* logfile;

log_handler::log_handler(int fd) : event_handler(fd) {
}

void log_handler::handle_read() {
    const char* types[2];
    types[msg::info-1] = "?";
    types[msg::err-1] = "x";

    int len = -1;
    char buf[128];

    if((len=read(get_handle(), buf, sizeof(int))) == sizeof(int)) {
        buf[len] = '\0';
        int size = *(int*)buf;
        if((len=recv(get_handle(), buf+sizeof(int), size, MSG_WAITALL)) == size) {
            struct msg* msg = (struct msg*)buf;
            char tmp[128];
            sprintf(tmp, "[%s]> %s %s\n", msg->name, types[msg->type-1], msg->data);
            fwrite(tmp, sizeof(char), strlen(tmp), logfile);
            fflush(logfile);
            std::cout << tmp << std::endl;
            return;
        }
    }
    if(len == 0)
        std::cout << "[!] client disconnected " << get_handle() << std::endl;
    else
        std::cout << "[x] error read from client " << get_handle() << ": " << strerror(errno) << std::endl;

    handle_disconnect();
}

void log_handler::handle_write() {
}

void log_handler::handle_disconnect() {
    epoll_reactor::instance()->remove_handler(get_handle());
    close(get_handle());
}
