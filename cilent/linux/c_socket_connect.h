#ifndef _C_SOCKET_CONNECT_H
#define _C_SOCKET_CONNECT_H

#include "c_send_file.h"
#include "c_recv_file.h"
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <queue>

class c_socket_connect{
    public:
        //c_socket_connect(std::string ipaddr,int port,c_send_file* _file,std::string username,std::string password);
        c_socket_connect(std::string ipaddr,int port,std::string username,std::string password);
        ~c_socket_connect();
        int exec_c_connect(c_send_file* c_file);
        int exec_c_connect(c_recv_file* c_file);
        int exec_c_connect();
        int log_c_connect();
        void set_c_func(int _op);
        c_send_file* c_file;
        std::string data_buf;
    private:
        std::string username,password;
        int sockfd;
        int op;
        int server_port;
        struct sockaddr_in cilent_addr;
    //    std::string data_buf;
};

#endif