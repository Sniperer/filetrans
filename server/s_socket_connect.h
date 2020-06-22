#ifndef _S_SOCKET_CONNECT_H
#define _S_SOCKET_CONNECT_H

#include "threadpool.h"
#include "s_recv_file.h"

const char[] s_save_file_dir="~/filetrans_data";

static int str_from_ith_cmp(const char* str1,const char* str2,int from,int to);
static int str_to_int(const char* str);

class s_socket_connect_task:public thread_task{
    public:
        s_socket_connect_task(int _fd,struct sockaddr* _addr,socklen_t* _addr_len);
        virtual int exec();
    private:
        s_socket_connect connect_proc;
};

class s_socket_connect_task_write:public s_socket_connect_task{
    public:
        virtual int exec();
};

class s_socket_connect{
    public:
        s_socket_connect(int _fd,struct sockaddr* _addr,socklen_t* _addr_len);
        ~s_socket_connect();
    
    private:
        int c_sockfd;
        struct sockaddr cilent_addr;
        socklen_t addr_len;
        int s_solve_connect();
        char data_buf[4096];
        s_recv_file s_file;
        
    //    pthread_mutex_t buf_mutex;

};
#endif