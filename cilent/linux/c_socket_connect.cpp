#include "c_socket_connect.h"

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

void int_to_ith_chars(std::string& _buf,int num){
    std::string tmp="";
    while(num!=0){
        tmp+=(char)('0'+num%10);
        num=num/10;
    }
    reverse(tmp.begin(),tmp.end());
    _buf+=tmp;
}
//c_socket_connect::c_socket_connect(std::string ipaddr,int port,c_send_file* _file,std::string username,std::string password)
//    :c_file(_file),username(username),password(password){
//    sockfd=socket(AF_INET,SOCK_STREAM,0);
//    memset(&cilent_addr,0,sizeof(cilent_addr));
//    cilent_addr.sin_family=AF_INET;
//    cilent_addr.sin_port=htons(port);
//    cilent_addr.sin_addr.s_addr=inet_addr(ipaddr.data());
//}

c_socket_connect::c_socket_connect(std::string ipaddr,int port,std::string username,std::string password)
    :username(username),password(password){
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    memset(&cilent_addr,0,sizeof(cilent_addr));
    cilent_addr.sin_family=AF_INET;
    cilent_addr.sin_port=htons(port);
    cilent_addr.sin_addr.s_addr=inet_addr(ipaddr.data());
    connect(sockfd,(struct sockaddr*)&cilent_addr,sizeof(cilent_addr));
}

int c_socket_connect::exec_c_connect(c_send_file* _file){
    
//    connect(sockfd,(struct sockaddr*)&cilent_addr,sizeof(cilent_addr));
//    if(log_c_connect()==0) return 2;
    c_file=_file;
    data_buf="FILETRANS/\r\n\r\n";
    pr_data(data_buf);
    send(sockfd,data_buf.data(),data_buf.size(),0);
    data_buf="FILESIZE:";
    int_to_ith_chars(data_buf,c_file->exec_c_send_file_size());
    data_buf+="\r\n";
    pr_data(data_buf);
    send(sockfd,data_buf.data(),data_buf.size(),0);
    data_buf="FILENAME:";
    data_buf+=c_file->exec_c_send_file_name();
    data_buf+="\r\n\r\n";
    pr_data(data_buf);
    send(sockfd,data_buf.data(),data_buf.size(),0);
    data_buf="\r\n\r\n";
    pr_data(data_buf);
    send(sockfd,data_buf.data(),data_buf.size(),0);
    int _len;
    while(_len=c_file->exec_c_send_file_data(data_buf,4096)){
        pr_int(_len);        
        pr_data(data_buf);
        send(sockfd,data_buf.data(),_len,0);
    }
    return 0;
//    send(sockfd,c_file->exec_c_send_file_size())
}

int c_socket_connect::log_c_connect(){
    data_buf="FILETRANS/login\r\n\r\n";
    send(sockfd,data_buf.data(),data_buf.size(),0);
    data_buf="USERNAME:";
    data_buf+=username;
    data_buf+="\r\n";
    send(sockfd,data_buf.data(),data_buf.size(),0);
    data_buf="PASSWORD:";
    data_buf+=password;
    data_buf+="\r\n\r\n";
    send(sockfd,data_buf.data(),data_buf.size(),0);
    char recv_buf[4096];
    int recv_len=recv(sockfd,recv_buf,4096,0);
    int i=0,flag=0;
    while(i<recv_len){
        if(recv_buf[i]=='\r'&&recv_buf[i+1]=='\n'&&recv_buf[i+2]=='\r'&&recv_buf[i+3]=='\n'){
            if(str_from_ith_cmp("SUCCESS",recv_buf,flag)==0){
                close(sockfd);
                return 1;
            }
            else{
                return 0;
            }
        }
        else i++;
    }
    close(sockfd);
    return 0;
}

void c_socket_connect::set_c_func(int _op){
    op=_op;
}