#include "event_handler.h"

event_handler::event_handler(int fd) {
    this->fd = fd;
}

int event_handler::get_handle() {
    return fd;
}

void event_handler::handle_connect() {
}

void event_handler::handle_disconnect() {
}

void event_handler::handle_timeout() {
}

void event_handler::handle_read() {
}

void event_handler::handle_write() {
}

