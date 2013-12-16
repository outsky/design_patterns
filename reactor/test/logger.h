#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include "../event_handler.h"

class log_handler : public event_handler {
    public:
        log_handler(int fd);

        void handle_read();
        void handle_write();
        void handle_disconnect();
};

#endif
