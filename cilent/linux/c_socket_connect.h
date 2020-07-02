#ifndef _C_SOCKET_CONNECT_H
#define _C_SOCKET_CONNECT_H

#include "c_send_file.h"
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <algorithm>
#include <iostream>

class c_socket_connect{
    public:
        c_socket_connect(std::string ipaddr,int port,c_send_file* _file,std::string username,std::string password);
        ~c_socket_connect();
        int exec_c_connect();
        int log_c_connect();
        c_send_file* c_file;
        std::string data_buf;
    private:
        std::string username,password;
        int sockfd;
        int server_port;
        struct sockaddr_in cilent_addr;
    //    std::string data_buf;
};

#endif