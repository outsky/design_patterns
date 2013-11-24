#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "server.h"
#include "msg.h"

FILE* logfile = NULL;

server::server(int port) {
    if( !listen(port) ) {
        std::cout << "[x] listen failed: " << strerror(errno) << std::endl;
        exit(-1);
    }
    if((logfile=fopen("log", "w")) == NULL) {
        std::cout << "[x] open log failed: " << strerror(errno) << std::endl;
        exit(-1);
    }
}

server::~server() {
    std::cout << "[?] bye." << std::endl;
    close(listenfd);
    fclose(logfile);
}

void* trd_read(void* p) {
    const char* types[2];
    types[msg::info-1] = "?";
    types[msg::err-1] = "x";

    int fd = *(int*)p;
    int len = -1;
    char buf[128];

    while((len=read(fd, buf, sizeof(int))) == sizeof(int)) {
        buf[len] = '\0';
        int size = *(int*)buf;
        if((len=recv(fd, buf+sizeof(int), size, MSG_WAITALL)) != size)
            break;

        struct msg* msg = (struct msg*)buf;
        char tmp[128];
        sprintf(tmp, "[%s]> %s %s\n", msg->name, types[msg->type-1], msg->data);
        fwrite(tmp, sizeof(char), strlen(tmp), logfile);
        fflush(logfile);
        std::cout << tmp << std::endl;
    }
    if(len == 0)
        std::cout << "[!] client disconnected " << fd << std::endl;
    else
        std::cout << "[x] error read from client " << fd << ": " << strerror(errno) << std::endl;
    close(fd);
    return NULL;
}

void server::run() {
    running = true;
    int fd = -1;
    while( running ) {
        if((fd=accept(listenfd, NULL, NULL)) == -1) {
            std::cout << "[!] accept: " << strerror(errno) << std::endl;
            continue;
        }
        std::cout << "[?] accept new connection " << fd << std::endl;

        pthread_t tid;
        if(pthread_create(&tid, NULL, trd_read, &fd) != 0) {
            std::cout << "[!] create read thread failed: " << strerror(errno) << std::endl;
            close(fd);
        }
        pthread_detach(tid);
    }
}

bool server::listen(int port) {
    if((listenfd=socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return false;

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(struct sockaddr_in));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons(port);
    if(bind(listenfd, (struct sockaddr*)&sa, sizeof(struct sockaddr_in)) == -1)
        return false;

    if(::listen(listenfd, 128) == -1)
        return false;

    return true;
}

void server::quit() {
    running = false;
}
