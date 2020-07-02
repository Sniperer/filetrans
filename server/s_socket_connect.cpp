#include "s_socket_connect.h"
#include <cstring>
//#define socklen_t int

#ifdef DEBUG
#define pr_data(str) printf("%s\n",str.c_str());
#define pr_int(len) printf("%d\n",len);
#else 
#define pr_data(str) 
#define pr_int(len) 
#endif 

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
        ans=ans*10+(int)(*(str+len)-'0');
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
    connect_proc->s_solve_login();
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
    FILENAME:$(name)'\r\n\r\n'
    '\r\n\r\n'
    $(filedata)
*/
int s_socket_connect::s_solve_connect(){
    int recv_len=recv(c_sockfd,data_buf,4096,0);
#ifdef DEBUG
    std::cout<<"first recv file...file size is "<<recv_len<<std::endl;
    std::cout<<data_buf<<std::endl;
    std::cout<<"header debug end."<<std::endl;
#endif
    int i=0,flag=0;
    bool have_head_bool=0,have_size_bool=0,have_name_bool=0;
    while(i<recv_len){
        if(data_buf[i]=='\r'&&data_buf[i+1]=='\n'){
            if(data_buf[i+2]=='\r'&&data_buf[i+3]=='\n'){
                if(have_head_bool==0&&str_from_ith_cmp("FILETRANS/",data_buf,flag)==0){
#ifdef DEBUG
    std::cout<<"FILETRANS head recieved..."<<std::endl;        
#endif
                    i=i+4;
                    flag=i;
                    have_head_bool=1;
                }
                else if(have_head_bool==1&&have_size_bool==1&&have_name_bool==1){
#ifdef DEBUG
    std::cout<<"file data recving..."<<std::endl;
#endif
                    i=i+4;
                    flag=i;
                    char _data[4096];
                    int j;
                    for(j=flag;j<recv_len;j++){
                        _data[j-flag]=data_buf[j];    
                    }
                    //_data[j-flag]='\0';
                    std::string str_data(_data);
                    s_file->s_recv_file_size(str_data.size());                    
                    //pr_data(str_data);
                    //pr_int(str_data.size());
                    s_file->s_recv_file_set_file_data(std::move(str_data));
                    pr_int(s_file->s_get_size_diff());
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
                else if(have_head_bool==1&&have_size_bool==1&&have_name_bool==0&&str_from_ith_cmp("FILENAME:",data_buf,flag)==0){
#ifdef DEBUG
    std::cout<<"file_name running..."<<std::endl;
#endif
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
                    i=i+4;
                    flag=i;
#ifdef DEBUG
    std::cout<<data_buf[i+4]<<std::endl;
#endif
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

/*
    FILETRANS/login'\r\n\r\n'
    USERNAME:$(user)'\r\n'
    PASSWORD:&(name)'\r\n\r\n'
*/
int s_socket_connect::s_solve_login(){
    char _username[256];
    char _password[256];
    int recv_len=recv(c_sockfd,data_buf,4096,0);
#ifdef DEBUG
    std::cout<<data_buf<<std::endl;
#endif
    int i=0,flag=0;
    bool have_head_bool=0,have_username_bool=0,have_password_bool=0;
    while(i<recv_len){
        if(data_buf[i]=='\r'&&data_buf[i+1]=='\n'){
            if(data_buf[i+2]=='\r'&&data_buf[i+3]=='\n'){
                if(have_head_bool==0&&str_from_ith_cmp("FILETRANS/login",data_buf,flag)==0){
#ifdef DEBUG
    std::cout<<"have head."<<std::endl;
#endif
                    i=i+4;
                    flag=i;
                    have_head_bool=1;
                }
                else if(have_head_bool==1&&have_username_bool==1&&have_password_bool==0&&str_from_ith_cmp("PASSWORD:",data_buf,flag)==0){
                    flag=flag+9;

                //    char _password[256];
                    int j;
                    for(j=flag;j<i;j++){
                        _password[j-flag]=data_buf[j];
                    }
                    _password[j-flag]='\0';
#ifdef DEBUG
    std::cout<<_password<<std::endl;
#endif
                    i=i+4;
                    flag=i;
                    have_password_bool=1;
                }
            }
            else{
                if(have_head_bool==1&&have_username_bool==0&&str_from_ith_cmp("USERNAME:",data_buf,flag)==0){
                    flag=flag+9;
                    std::cout<<"???"<<std::endl;
                    int j;
                    for(j=flag;j<i;j++){
                        _username[j-flag]=data_buf[j];
                    }
                    _username[j-flag]='\0';
#ifdef DEBUG
    std::cout<<_username<<std::endl;
#endif
                    i=i+2;
                    flag=i;
                    have_username_bool=1;
                }
            }
        }
        else i++;
    }
    std::fstream fs2;
    fs2.open("fileserver.conf",std::ios::in|std::ios::out);
    if(!fs2.is_open()){
#ifdef DEBUG
    std::cout<<"file not exist."<<std::endl;
#endif
        return 1;
    }
    std::string cfg_buf(256,'\0');
    std::string username(256,'\0'),password(256,'\0');
    while(fs2.getline(&cfg_buf[0],256)){
        if(str_from_ith_cmp("username",&cfg_buf[0],0)==0){
            int i=9;
            while(cfg_buf[i]!='\0'){
                username[i-9]=cfg_buf[i];   
                i++;     
            }
            username.resize(i-9);
        }
        else if(str_from_ith_cmp("password",&cfg_buf[0],0)==0){
            int i=9;
            while(cfg_buf[i]!='\0'){
                password[i-9]=cfg_buf[i];
                i++;
            }
            password.resize(i-9);
        }
    }
#ifdef DEBUG
    std::cout<<password<<":"<<_username<<" "<<username<<":"<<_password<<std::endl;
#endif
    fs2.close();
    if(username==_username && password==_password){
#ifdef DEBUG
    std::cout<<"success"<<std::endl;
#endif
        s_send_suclogin();
        s_solve_connect();
    }
    else{
#ifdef DEBUG
    std::cout<<"error"<<std::endl;
#endif
        s_send_errlogin();
        close(c_sockfd);
        return 1;          
    }
    close(c_sockfd);
    return 0;
}

/*
    ERROR\r\n
*/
void s_socket_connect::s_send_errlogin(){
    std::string err_login="ERROR";
    err_login+="\r\n\r\n";
    send(c_sockfd,err_login.data(),err_login.size(),0);
}

/*
    SUCCESS\r\n
*/
void s_socket_connect::s_send_suclogin(){
    std::string suc_login="SUCCESS";
    suc_login+="\r\n\r\n";
    send(c_sockfd,suc_login.data(),suc_login.size(),0);
}