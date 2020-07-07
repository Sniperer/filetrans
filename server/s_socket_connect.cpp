#include "s_socket_connect.h"
#include <cstring>
//#define socklen_t int

#ifdef DEBUG
#define pr_data(str) printf("%s\n",str.c_str());
#define pr_c_data(str) printf("%s\n",str);
#define pr_int(len) printf("%d\n",len);
#else 
#define pr_c_data(str) 
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

static void int_to_ith_chars(std::string& _buf,int num){
    std::string tmp="";
    while(num!=0){
        tmp+=(char)('0'+num%10);
        num=num/10;
    }
    reverse(tmp.begin(),tmp.end());
    _buf+=tmp;
}

s_socket_connect_task::s_socket_connect_task(threadpool* tp,int _fd,struct sockaddr_in _addr,socklen_t _addr_len):thread_task(tp){
    //thread_task(tp);
    connect_proc=new s_socket_connect(_fd,_addr,_addr_len);
}

int s_socket_connect_task::exec(){
#ifdef DEBUG
    std::cout<<"s_socket_connect_task running..."<<std::endl;
#endif
    connect_proc->s_doit();
    return 0;
}

s_socket_connect::s_socket_connect(int _fd,struct sockaddr_in _addr,socklen_t _addr_len):c_sockfd(_fd){
    _cilent_addr=_addr;
    addr_len=_addr_len;
}

/*
    FILETRANS/upload'\r\n\r\n'
    FILESIZE:$(len)'\r\n'
    FILENAME:$(name)'\r\n\r\n'
    '\r\n\r\n'
    $(filedata)

    error return 1;else return 0;
*/
int s_socket_connect::s_solve_upload(int len){
//    int recv_len=recv(c_sockfd,data_buf,4096,0);
    int recv_len=len;
#ifdef DEBUG
    std::cout<<"first recv file...file size is "<<recv_len<<std::endl;
    std::cout<<data_buf<<std::endl;
    std::cout<<"header debug end."<<std::endl;
#endif
    int i=0,flag=0;
    bool have_head_bool=0,have_size_bool=0,have_name_bool=0;
    s_recv_file *s_file=new s_recv_file();
    int f_len;
    while(i<recv_len){
        if(data_buf[i]=='\r'&&data_buf[i+1]=='\n'){
            if(data_buf[i+2]=='\r'&&data_buf[i+3]=='\n'){
                if(have_head_bool==0&&str_from_ith_cmp("FILETRANS/upload",data_buf,flag)==0){
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
                    for(j=flag;j<flag+f_len;j++){
                        _data[j-flag]=data_buf[j];    
                    }
                    _data[j-flag]='\0';
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
                            for(int i=0;i<4096;i++)
                                data_buf[i]='\0';
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
                    f_len=str_to_int(data_buf+flag,i-flag);
                    s_file->s_recv_file_set_file_size(f_len);
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

    error return 1,else return 0;
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
        for(int i=0;i<recv_len;i++)
            data_buf[i]='\0';
        s_send_suclogin();
    //    s_solve_connect();
        return 0;
    }
    else{
#ifdef DEBUG
    std::cout<<"error"<<std::endl;
#endif
        for(int i=0;i<recv_len;i++)
            data_buf[i]='\0';
        s_send_errlogin();
    //    close(c_sockfd);
        return 1;          
    }
    //close(c_sockfd);
    //return 0;
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

int s_socket_connect::s_solve_ls(){
    std::string re_buf="FILETRANS/ls\r\n\r\n";
    std::string files="";
    std::string dir_path=getenv("HOME");
    dir_path+="/filetrans_data";
    DIR *s_dir=opendir(dir_path.data());    
    struct dirent *dir_pt;
    int num=0;
    while((dir_pt=readdir(s_dir))!=NULL){
        num++;
        files+=dir_pt->d_name;
        files+="\r\n";
    }
    re_buf=re_buf+"FILENUMS:";
    int_to_ith_chars(re_buf,num);
    re_buf+="\r\n\r\n"+files;
    send(c_sockfd,re_buf.data(),re_buf.size(),0);
    return 0;
}

int s_socket_connect::s_solve_download(int len){
    int recv_len=len;
    int i=0,flag=0;
    std::string re_buf;
    while(i<recv_len){
        if(str_from_ith_cmp("\r\n\r\n",data_buf,i)==0){
            if(str_from_ith_cmp("FILETRANS/download",data_buf,flag)==0){
                i=i+4;
                flag=i;
            }
            else if(str_from_ith_cmp("FILENAME:",data_buf,flag)==0){
                flag=flag+9;
                std::string file_name=getenv("HOME");
                file_name+="/filetrans_data/";
                for(int j=flag;j<i;j++)
                    file_name+=data_buf[j];
                pr_data(file_name);
                s_send_file *s_file=new s_send_file(file_name);
                if(s_file->exec_s_send_file_size()<0){
                    pr_int(1122333);
                    re_buf="FILETRANS/ 0\r\n\r\n";
                    send(c_sockfd,re_buf.data(),re_buf.size(),0);
                    return 1;
                }
                else{
                    re_buf="FILETRANS/ 1\r\n\r\n";
                    pr_data(re_buf);
                    send(c_sockfd,re_buf.data(),re_buf.size(),0);
                    re_buf="FILESIZE:";
                    pr_int(s_file->exec_s_send_file_size());
                    int_to_ith_chars(re_buf,s_file->exec_s_send_file_size());
                    re_buf+="\r\n\r\n";
                    //pr_data(re_buf);
                    send(c_sockfd,re_buf.data(),re_buf.size(),0);
                    re_buf="\r\n\r\n";
                    //send(c_sockfd,re_buf.data(),re_buf.size(),0);
                    //pr_data(re_buf);
                    send(c_sockfd,re_buf.data(),re_buf.size(),0);
                    //pr_data(re_buf);
                    int n=s_file->exec_s_send_file_data(re_buf,4096);
                    pr_data(re_buf);
                    send(c_sockfd,re_buf.data(),n,0);
                
                    return 0;
                }
            }
        }
        else{
            i++;
        }
    }
    return 1;
}

int s_socket_connect::s_doit(){
    while(1){
        std::cout<<"logining..."<<std::endl;
        if(s_solve_login()==0){
            status=1;
            break;
        }
    }
    int recv_len=recv(c_sockfd,data_buf,4096,0);
    pr_int(recv_len);
    pr_c_data(data_buf);
    for(int i=0;i<recv_len;i++){
        if(str_from_ith_cmp("\r\n\r\n",data_buf,i)==0){
            if(str_from_ith_cmp("FILETRANS/upload",data_buf,0)==0){
#ifdef DEBUG
    printf("upload\n");                
#endif
                s_solve_upload(recv_len);
                break;
            }
            else if(str_from_ith_cmp("FILETRANS/download",data_buf,0)==0){
#ifdef DEBUG1
    printf("download\n");
#endif
                s_solve_download(recv_len);
                break;
            }
            else if(str_from_ith_cmp("FILETRANS/ls",data_buf,0)==0){
#ifdef DEBUG
    printf("ls\n");
#endif
                s_solve_ls();
                break;
            }
        }
        else{
            i++;
        }
    }
    close(c_sockfd);
    return 0;
}