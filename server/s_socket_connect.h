#ifndef _S_SOCKET_CONNECT_H
#define _S_SOCKET_CONNECT_H

#include "threadpool.h"
#include "s_recv_file.h"
#include "s_send_file.h"
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <dirent.h>

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
        ~s_socket_connect()=default;
        int s_solve_login();
        void s_send_errlogin();
        void s_send_suclogin();
        int s_solve_upload(int len);
        int s_solve_download(int len);
        int s_doit();
        int s_solve_ls();
    //    s_recv_file* s_file;
    private:
        int c_sockfd;
        bool status=0;
        struct sockaddr_in _cilent_addr;
        socklen_t addr_len;
    //    int s_solve_connect();
        char data_buf[4096];
    //    s_recv_file* s_file;
        
    //    pthread_mutex_t buf_mutex;

};
#endif