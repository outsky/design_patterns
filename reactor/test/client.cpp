#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "client.h"
#include "msg.h"

client::client(const char* s, const char* ip, int port) {
    if( !connect(ip, port) ) {
        std::cout << "[x] connect server failed: " << strerror(errno) << std::endl;
        exit(-1);
    }
    name = strdup(s);
}

client::~client() {
    free(name);
    name = NULL;
    close(sockfd);
}

bool client::connect(const char* ip, int port) {
    if((sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return false;

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(struct sockaddr_in));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(ip);
    sa.sin_port = htons(port);
    if(::connect(sockfd, (struct sockaddr*)&sa, sizeof(struct sockaddr_in)) == -1)
        return false;

    return true;
}

void client::log_info(const char* str) {
    if( !log(msg::info, str) )
        std::cout << "[!] write:" << strerror(errno) << std::endl;
    else
        std::cout << "[?] info:" << str << std::endl;
}

void client::log_err(const char* str) {
    if( !log(msg::err, str) )
        std::cout << "[!] write:" << strerror(errno) << std::endl;
    else
        std::cout << "[?] err:" << str << std::endl;
}

bool client::log(int t, const char* str) {
    int len = sizeof(struct msg) + strlen(str);
    struct msg* info = (struct msg*)new char[len];
    memset(info, 0, len);
    info->size = len-sizeof(int);
    info->type = t;
    strcpy(info->name, getname());
    strcpy(info->data, str);

    int ret = send(sockfd, info, len, 0);
    delete [] info;
    info = NULL;

    return ret==len;
}
