#include "s_socket_connect.h"
#include <cstring>

static int str_from_ith_cmp(const char* str1,const char* str2,int ith){
    int len=0;
    while(*(str1+len)!='\0'){
        len++;
    }
    for(int i=0;i<len;i++){
        if(*(str1+i)!=*(str2+ith+i)){
            return 1;
        }
    }
    return 0;
}

static int str_to_int(const char* str){
    int len=0,ans=0;
    while(*(str+len)!='\0'){
        ans=ans*10+(int)(*(str+len));
        len++;
    }
    return ans;
}

s_socket_connect_task::s_socket_connect_task(int _fd,struct sockaddr* _addr,socklen_t* _addr_len){
    connect_proc=new s_socket_connect(_fd,_addr,_addr_len);
}

int s_socket_connect_task::exec(){
//    connect_proc=new s_socket_connect()
    s_solve_connect();
    connect_proc->s_file.s_mk_file();
    delete connect_proc;
}

s_socket_connect::s_socket_connect(int _fd,struct sockaddr* _addr,socklen_t* _addr_len):c_sockfd(_fd){
    cilent_addr=*_addr;
    addr_len=*_addr_len;

}

s_socket_connect::~s_socket_connect(){

}

/*
    FILETRANS/'\r\n\r\n'
    FILESIZE:$(len)'\r\n'
    FILENAME:$(name)'\r\n'
    '\r\n\r\n'
    $(filedata)
*/
int s_socket_connect::s_solve_connect(){
    int recv_len=recv(c_sockfd,data_buf,4096,0);
    int i=0,flag=0;
    bool have_head_bool=0,have_size_bool=0,have_name_bool=0;
    while(i<recv_len){
        if(data_buf[i]=='\r'&&data_buf[i+1]=='\n'){
            if(data_buf[i+2]=='\r'&&data_buf[i+3]=='\n'){
                if(have_head_bool==0&&str_from_ith_cmp("FILETRANS/",data_buf[flag])==0){
                    i=i+4;
                    flag=i;
                    have_head_bool=1;
                }
                else if(have_head_bool==1&&have_size_bool==1&&have_name_bool==1&&str_from_ith_cmp("",data_buf[flag])==0){

                }   
                else{
                    /*
                        Miss information.
                    */
                }             
            }
            else{
                if(have_head_bool==1&&have_size_bool==0&&str_from_ith_cmp("FILESIZE:",data_buf[flag])==0){
                    str_to_int()
                    have_size_bool=1;
                }
                else if(have_head_bool==1&&have_size_bool==1&&hace_name_bool==0&&str_from_ith_cmp("FILENAME:",data_buf[flag])==0){
                    
                    have_name_bool=1;
                }
                else{
                    /*
                        Miss information.
                    */
                }
            }
        }
        else{
            i++;
        }    
    }
}

int s_socket_connect_task_write::exec(){
    connect_proc
}