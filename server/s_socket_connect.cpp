#include "s_socket_connect.h"
#include <cstring>
//#define socklen_t int

static int str_from_ith_cmp(const char* str1,char* str2,int ith){
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

static int str_to_int(char* str,int num){
    int len=0,ans=0;
    while(len<num){
        ans=ans*10+(int)(*(str+len));
        len++;
    }
    return ans;
}

s_socket_connect_task::s_socket_connect_task(threadpool* tp,int _fd,struct sockaddr_in _addr,socklen_t _addr_len):thread_task(tp){
    //thread_task(tp);
    connect_proc=new s_socket_connect(_fd,_addr,_addr_len);
}

int s_socket_connect_task::exec(){
#ifdef DEBUG
    std::cout<<"s_socket_connect_task running..."<<std::endl;
#endif
    connect_proc->s_solve_connect();
    return 0;
}

s_socket_connect::s_socket_connect(int _fd,struct sockaddr_in _addr,socklen_t _addr_len):c_sockfd(_fd){
    _cilent_addr=_addr;
    addr_len=_addr_len;
    s_file=new s_recv_file();
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
#ifdef DEBUG
    for(int i=0;i<recv_len;i++){
        std::cout<<data_buf[i];
    }
    std::cout<<std::endl;
#endif
    int i=0,flag=0;
    bool have_head_bool=0,have_size_bool=0,have_name_bool=0;
    while(i<recv_len){
        if(data_buf[i]=='\r'&&data_buf[i+1]=='\n'){
            if(data_buf[i+2]=='\r'&&data_buf[i+3]=='\n'){
                if(have_head_bool==0&&str_from_ith_cmp("FILETRANS/",data_buf,flag)==0){
#ifdef DEBUG
    std::cout<<"FILETRANS/"<<std::endl;        
#endif
                    i=i+4;
                    flag=i;
                    have_head_bool=1;
                }
                else if(have_head_bool==1&&have_size_bool==1&&have_name_bool==1&&str_from_ith_cmp("",data_buf,flag)==0){
                    i=i+4;
                    flag=i;
                    s_file->s_recv_file_size(recv_len-flag-1);                    
                    char _data[4096];
                    int j;
                    for(j=flag;j<flag+s_file->s_recv_file_get_file_size();j++){
                        _data[j-flag]=data_buf[j];    
                    }
                    _data[j-flag]='\0';
                    std::string str_data(_data);
                    s_file->s_recv_file_set_file_data(std::move(str_data));
                    while(s_file->s_get_size_diff()>=0){
                        if(s_file->s_get_size_diff()==0){
                            return 0;
                        }
                        else{
                            int _size=recv(c_sockfd,data_buf,4096,0);
                            s_file->s_recv_file_size(_size);
                            s_file->s_recv_file_set_file_data(data_buf);
                        }

                    }
                }   
                else{
                    /*
                        Miss information.
                    */
                }             
            }
            else{
                if(have_head_bool==1&&have_size_bool==0&&str_from_ith_cmp("FILESIZE:",data_buf,flag)==0){
                    flag=flag+9;
                    s_file->s_recv_file_set_file_size(str_to_int(data_buf+flag,i-flag));
#ifdef DEBUG
    std::cout<<str_to_int(data_buf+flag,i-flag)<<std::endl;
#endif
                    have_size_bool=1;
                    i=i+2;
                    flag=i;
                }
                else if(have_head_bool==1&&have_size_bool==1&&have_name_bool==0&&str_from_ith_cmp("FILENAME:",data_buf,flag)==0){
                    flag=flag+9;
                    char _name[256];
                    int j;
                    for(j=flag;j<i;j++){
                        _name[j-flag]=data_buf[j];
                    }
                    _name[j-flag]='\0';
                    std::string file_name(_name);
#ifdef DEBUG                    
    std::cout<<file_name<<std::endl;
#endif                    
                    s_file->s_recv_file_set_file_name(std::move(file_name));
                    have_name_bool=1;
                    i=i+2;
                    flag=i;
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
    return 1;
}