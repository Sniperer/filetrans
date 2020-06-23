#include "s_socket_listen.h"

s_socket_listen::s_socket_listen(int port):server_port(port){
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(server_port);
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
}

int s_socket_listen::exec_s_socket_listen(threadpool* tp){
    listen(sockfd,5);
    //int c_sockfd;
    struct sockaddr_in cilent_addr;
    socklen_t addr_len;
    addr_len=sizeof(cilent_addr);
    while(1){
        int c_sockfd=accept(sockfd,(struct sockaddr*)&cilent_addr,(socklen_t*)&addr_len);
#ifdef DEBUG
        std::cout<<"socket accepted..."<<std::endl;
#endif
        s_socket_connect_task* new_connect_task=new s_socket_connect_task(tp,c_sockfd,cilent_addr,addr_len);    
        tp->add_task(new_connect_task);
    }
    return 1;
}

s_socket_listen_task::s_socket_listen_task(threadpool *tp):thread_task(tp){
    listen_proc=new s_socket_listen(8848);
}

int s_socket_listen_task::exec(){
#ifdef DEBUG
    std::cout<<"s_socket_listen_task running..."<<std::endl;
#endif
    listen_proc->exec_s_socket_listen(tp);
    return 0;
}