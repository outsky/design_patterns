#ifndef CLIENT_H
#define CLIENT_H

class client {
    public:
        client(const char* s, const char* ip, int port);
        ~client();

        inline const char* getname() { return name; }
        inline int getsock() { return sockfd; }

        void log_info(const char* str);
        void log_err(const char* str);

    private:
        bool connect(const char* ip, int port);
        bool log(int t, const char* str);

    private:
        char* name;
        int sockfd;
};

#endif
