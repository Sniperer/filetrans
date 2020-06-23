#ifndef _S_SOCKETLISTEN_H
#define _S_SOCKETLISTEN_H

#include "threadpool.h"
#include "s_socket_connect.h"
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

class s_socket_listen;
class s_socket_listen_task;

class s_socket_listen_task:public thread_task{
    private:
        s_socket_listen *listen_proc;
    public:
        virtual int exec();
        s_socket_listen_task(threadpool *tp);
};

class s_socket_listen{
    public:
        s_socket_listen(int port);
        ~s_socket_listen()=default;

        int exec_s_socket_listen(threadpool* tp);
    private:
        int sockfd;
        int server_port;
        struct sockaddr_in server_addr;
};
#endif