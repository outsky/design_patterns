#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "../event_handler.h"

class accept_handler : public event_handler {
    public:
        accept_handler(int fd);

        void handle_read();
};

#endif
