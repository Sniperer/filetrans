#include "c_socket_connect.h"

#ifdef DEBUG
#define pr_data(str) printf("%s\n",str.c_str());
#define pr_int(len) printf("%d\n",len);
#else 
#define pr_data(str) 
#define pr_int(len) 
#endif 

void int_to_ith_chars(std::string& _buf,int num){
    std::string tmp="";
    while(num!=0){
        tmp+=(char)('0'+num%10);
        num=num/10;
    }
    reverse(tmp.begin(),tmp.end());
    _buf+=tmp;
}
c_socket_connect::c_socket_connect(int port,c_send_file* _file):c_file(_file){
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    memset(&cilent_addr,0,sizeof(cilent_addr));
    cilent_addr.sin_family=AF_INET;
    cilent_addr.sin_port=htons(port);
    cilent_addr.sin_addr.s_addr=inet_addr("192.168.1.102");
}

int c_socket_connect::exec_c_connect(){
    connect(sockfd,(struct sockaddr*)&cilent_addr,sizeof(cilent_addr));
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