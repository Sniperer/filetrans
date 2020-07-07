#include "c_socket_connect.h"

#ifdef DEBUG
#define pr_data(str) printf("%s\n",str.c_str());
#define pr_int(len) printf("%d\n",len);
#define pr_c_data(str) printf("%s\n",str);
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
//c_socket_connect::c_socket_connect(std::string ipaddr,int port,c_send_file* _file,std::string username,std::string password)
//    :c_file(_file),username(username),password(password){
//    sockfd=socket(AF_INET,SOCK_STREAM,0);
//    memset(&cilent_addr,0,sizeof(cilent_addr));
//    cilent_addr.sin_family=AF_INET;
//    cilent_addr.sin_port=htons(port);
//    cilent_addr.sin_addr.s_addr=inet_addr(ipaddr.data());
//}


/*
    FILETRANS/upload'\r\n\r\n'
    FILESIZE:$(len)'\r\n\r\n'
    FILENAME:$(name)'\r\n\r\n'
    '\r\n\r\n'
    &(filedata)
*/
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
    data_buf="FILETRANS/upload\r\n\r\n";
    //pr_data(data_buf);
    //send(sockfd,data_buf.data(),data_buf.size(),0);
    data_buf+="FILESIZE:";
    int_to_ith_chars(data_buf,c_file->exec_c_send_file_size());
    data_buf+="\r\n";
    //pr_data(data_buf);
    //send(sockfd,data_buf.data(),data_buf.size(),0);
    data_buf+="FILENAME:";
    data_buf+=c_file->exec_c_send_file_name();
    data_buf+="\r\n\r\n";
    //pr_data(data_buf);
    //send(sockfd,data_buf.data(),data_buf.size(),0);
    data_buf+="\r\n\r\n";
    pr_data(data_buf);
    send(sockfd,data_buf.data(),data_buf.size(),0);
    int _len;
    while((_len=c_file->exec_c_send_file_data(data_buf,4096))){
        pr_int(_len);        
        pr_data(data_buf);
        send(sockfd,data_buf.data(),_len,0);
    }
    return 0;
//    send(sockfd,c_file->exec_c_send_file_size())
}

/*
    FILETRANS/login'\r\n\r\n'
    USERNAME:$(user)'\r\n'
    PASSWORD:&(name)'\r\n\r\n'
*/
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
            //    close(sockfd);
                return 1;
            }
            else{
                return 0;
            }
        }
        else i++;
    }
    //close(sockfd);
    return 0;
}

void c_socket_connect::set_c_func(int _op){
    op=_op;
}

/*
    FILETRANS/download'\r\n\r\n'
    FILENAME:$(name)'\r\n\r\n'

*******************************************
    FILETRANS/ [0/1]'\r\n\r\n'
    FILESIZE:$(len)'\r\n\r\n'
    '\r\n\r\n'
    $(filedata)
*/
int c_socket_connect::exec_c_connect(c_recv_file* c_file){
    data_buf="FILETRANS/download\r\n\r\n";
    data_buf=data_buf+"FILENAME:"+c_file->c_recv_file_get_s_file_name()+"\r\n\r\n";
    send(sockfd,data_buf.data(),data_buf.size(),0);
    char recv_buf[4096];
    int recv_len=recv(sockfd,recv_buf,4096,0);
    //pr_c_data(recv_buf);
    int i=0,flag=11;
    if(recv_buf[flag]=='1'){
        //pr_int(123456789);
        flag=flag+5;
        i=flag;
        flag=flag+9;
        int f_len;
        while(1){
            if(recv_buf[i]=='\r'&&recv_buf[i+1]=='\n'&&recv_buf[i+2]=='\r'&&recv_buf[i+3]=='\n'){
                f_len=str_to_int(recv_buf+flag,i-flag);
                c_file->c_recv_file_set_file_size(f_len);
                break;
            }
            else i++;
        }
        pr_int(str_to_int(recv_buf+flag,i-flag));
        i=i+8;
        flag=i;
        char _data[4096];
        int j;
        for(j=flag;j<flag+f_len;j++){
            _data[j-flag]=recv_buf[j];
        }
        _data[f_len]='\0';
        std::string str_data(_data);
        pr_data(str_data);
        printf("%d\n",str_data.size());
        c_file->c_recv_file_size(str_data.size());
        c_file->c_recv_file_set_file_data(std::move(str_data));
        while(c_file->c_get_size_diff()>=0){
            if(c_file->c_get_size_diff()==0){
                return 0;
            }
            else{
                for(int i=0;i<4096;i++)
                    recv_buf[i]='\0';
                int _size=recv(sockfd,recv_buf,4096,0);
                for(int i=0;i<_size;i++)
                    str_data[i]=recv_buf[i];
                c_file->c_recv_file_size(_size);
                c_file->c_recv_file_set_file_data(std::move(str_data));
            }
        }
        return 1;
    }
    else{
        pr_int(123456780);
        return 0;
    }
}

/*
    FILETRANS/ls'\r\n\r\n'
********************************
    FILETRANS/'\r\n\r\n'
    FILENUMS:$(num)\r\n\r\n
    $(file)\r\n
    $(file)\r\n 
*/
int c_socket_connect::exec_c_connect(){
    std::string data_buf="FILETRANS/ls\r\n\r\n";
    send(sockfd,data_buf.data(),data_buf.size(),0);
    char recv_buf[4096];
    int recv_len=recv(sockfd,recv_buf,4096,0);
#ifdef DEBUG1
    printf("recv_len:%d\n%s\n",recv_len,recv_buf);
#endif
    int i=0,flag=0;
    bool have_head_bool=0,have_nums_bool=0;
    int file_num=0;
    std::queue<std::string> v;
    std::string tmp;
    while(i<recv_len){
        if(recv_buf[i]=='\r'&&recv_buf[i+1]=='\n'){
            if(recv_buf[i+2]=='\r'&&recv_buf[i+3]=='\n'){
                if(have_head_bool==0&&str_from_ith_cmp("FILETRANS/",recv_buf,flag)==0){
                    i=i+4;
                    flag=i;
                    have_head_bool=1;
                }
                else if(have_head_bool==1&&have_nums_bool==0&&str_from_ith_cmp("FILENUMS:",recv_buf,flag)==0){
                    flag=flag+9;
                    file_num=str_to_int(recv_buf+flag,i-flag);
                    i=i+4;
                    flag=i;
                    have_nums_bool=1;
                }
            }
            else{
                tmp="";
                for(int j=flag;j<i;j++)
                    tmp+=recv_buf[j];
                v.push(tmp);
                i=i+2;
                flag=i;
            }
        }
        else
            i++;
    }
    std::cout<<file_num<<std::endl;
    while(!v.empty()){
        std::cout<<v.front()<<std::endl;
        v.pop();
    }
    return 0;
}