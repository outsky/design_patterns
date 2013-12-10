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

#include "../epoll_reactor.h"
#include "acceptor.h"
#include "logger.h"

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

void server::run() {
    running = true;

    // register accept handler to accept new connections
    accept_handler* ah = new accept_handler(listenfd);
    epoll_reactor::instance()->register_handler(event_handler::CONNECT, ah);

    // events demultiplex and dispatch
    while( running )
        epoll_reactor::instance()->handle_events();
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
