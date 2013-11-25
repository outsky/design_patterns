#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>

#include "server_handler.h"
#include "../reactor.h"
#include "msg.h"

extern FILE* logfile;

accept_handler::accept_handler(int fd) {
    handle = fd;
}

void accept_handler::handle_read() {
    int fd = -1;
    if((fd=accept(handle, NULL, NULL)) == -1) {
            std::cout << "[!] accept: " << strerror(errno) << std::endl;
            return;
    }
    std::cout << "[?] accept new connection " << fd << std::endl;

    log_handler* handler = new log_handler(fd);
    reactor::instance()->register_handler(READ, handler);
}

log_handler::log_handler(int fd) {
    handle = fd;
}

void log_handler::handle_read() {
    const char* types[2];
    types[msg::info-1] = "?";
    types[msg::err-1] = "x";

    int len = -1;
    char buf[128];

    if((len=read(handle, buf, sizeof(int))) == sizeof(int)) {
        buf[len] = '\0';
        int size = *(int*)buf;
        if((len=recv(handle, buf+sizeof(int), size, MSG_WAITALL)) == size) {
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
        std::cout << "[!] client disconnected " << handle << std::endl;
    else
        std::cout << "[x] error read from client " << handle << ": " << strerror(errno) << std::endl;
    reactor::instance()->remove_handler(handle);
    close(handle);
}

void log_handler::handle_write() {

}
