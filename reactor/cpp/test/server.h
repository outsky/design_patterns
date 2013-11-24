#ifndef SERVER_H
#define SERVER_H

class server {
    public:
        server(int port);
        ~server();

        void run();
        void quit();

    private:
        bool listen(int port);

    private:
        int listenfd;
        bool running;
};

#endif
