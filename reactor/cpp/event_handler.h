#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

class event_handler {
     public:
        enum {
            CONNECT     = 1<<0,
            DISCONNECT  = 1<<1,
            READ        = 1<<2,
            WRITE       = 1<<3,
            TIMEOUT     = 1<<4
        };

    public:
        event_handler(int fd);

        virtual void handle_connect();
        virtual void handle_disconnect();
        virtual void handle_timeout();
        virtual void handle_read();
        virtual void handle_write();

        int get_handle();

    private:
        int fd;
};

#endif
