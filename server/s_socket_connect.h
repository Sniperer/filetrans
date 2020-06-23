#ifndef _S_SOCKET_CONNECT_H
#define _S_SOCKET_CONNECT_H

#include "threadpool.h"
#include "s_recv_file.h"
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

//const char[] s_save_file_dir="~/filetrans_data";

class s_socket_connect_task;
class s_socket_connect;
//#define socklen_t int

class s_socket_connect_task:public thread_task{
    public:
        s_socket_connect_task(threadpool* tp,int _fd,struct sockaddr_in _addr,socklen_t _addr_len);
        virtual int exec();
    private:
        s_socket_connect *connect_proc;
};

class s_socket_connect_task_write:public s_socket_connect_task{
    public:
        virtual int exec();
};

class s_socket_connect{
    public:
        s_socket_connect(int _fd,struct sockaddr_in _addr,socklen_t _addr_len);
        ~s_socket_connect();
        int s_solve_connect();
        s_recv_file* s_file;
    private:
        int c_sockfd;
        struct sockaddr_in _cilent_addr;
        socklen_t addr_len;
    //    int s_solve_connect();
        char data_buf[4096];
    //    s_recv_file* s_file;
        
    //    pthread_mutex_t buf_mutex;

};
#endif