#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include "server.h"

server* ps = NULL;
void quit(int);

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cout << "[x] USAGE: " << argv[0] << " [port]" << std::endl;
        exit(-1);
    }

    struct sigaction sa;
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = quit;
    sigaction(SIGINT, &sa, NULL);

    server s(atoi(argv[1]));
    ps = &s;
    s.run();

    return 0;
}

void quit(int s) {
    if(s != SIGINT)
        return;

    ps->quit();
}
