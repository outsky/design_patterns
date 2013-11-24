#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>

#include "client.h"

void* trd_read(void*);

int main(int argc, char* argv[]) {
    if(argc != 4) {
        std::cout << "[x] USAGE: " << argv[0] << " [name] [ip] [port]" << std::endl;
        exit(-1);
    }

    const char* name = argv[1];
    const char* ip = argv[2];
    int port = atoi(argv[3]);
    client c(name, ip, port);

    pthread_t pid;
    int fd = c.getsock();
    pthread_create(&pid, NULL, trd_read, &fd);
    pthread_detach(pid);

    srand(time(NULL));
    char buf[1024];
    while( true ) {
        time_t now = time(NULL);

        sprintf(buf, "%s", ctime(&now));
        if(rand()%100 < 80)
            c.log_info(buf);
        else
            c.log_err(buf);

        sleep(rand()%10);
    }

    return 0;
}

void* trd_read(void* p) {
    int fd = *(int*)p;
    char buf[128];
    if(recv(fd, buf, 128, MSG_WAITALL) == 0) {
        std::cout << "[x] server disconnected." << std::endl;
        exit(-1);
    }
    return NULL;
}
