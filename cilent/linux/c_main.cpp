#include <iostream>
#include <string>
#include "c_send_file.h"
#include "c_socket_connect.h"
#include "c_recv_file.h"

int main(int argc,char* argv[]){
    std::string ip_addr,username,password,srcpath,dstpath;
    int op;
    std::cout<<">filetrans start.\n";
    std::cout<<">input ip address:";
    std::cin>>ip_addr;
    c_socket_connect *c_con;
    while(1){
        std::cout<<">input username:";
        std::cin>>username;
        std::cout<<">input password:";
        std::cin>>password;
        c_con=new c_socket_connect(ip_addr,8848,username,password);
        if(c_con->log_c_connect()) break;
        std::cout<<">wrong username OR password.\n";
    }
    std::cout<<">input function[1/2/3](upload file/download file/show files):";
    std::cin>>op;
    c_con->set_c_func(op);
    if(op==1){
        /*upload file*/
        std::cout<<">input cilent filepath:";
        std::cin>>srcpath;
        std::cout<<">input server filename:";
        std::cin>>dstpath;
        c_send_file *fd=new c_send_file(srcpath,dstpath);
        c_con->exec_c_connect(fd);
    }
    else if(op==2){
        /*download file*/
        std::cout<<">input server filepath:";
        std::cin>>srcpath;
        std::cout<<">input cilent filename:";
        std::cin>>dstpath;
        
    }
    else if(op==3){
        /*show files*/

    }
    c_socket_connect *c_con=new c_socket_connect(ip_addr,8848,fd,username,password);
    if(c_con->exec_c_connect()==2){
        std::cout<<">wrong username OR password. program exit."<<std::endl;
        return 0;
    }

/*    
    if(argc==1||argc%2==0){
        std::cout<<"please input file path."<<std::endl;
        return 0;
    }
    else{
    //    c_socket_connect c_con(8848);
        for(int i=1;i<argc;i=i+2){
            std::cout<<"file path:";
            std::cout<<argv[i]<<argv[i+1]<<std::endl;
            c_send_file *fd=new c_send_file(argv[i],argv[i+1]);
            c_socket_connect *c_con=new c_socket_connect(ip_addr,8848,fd);
            c_con->exec_c_connect();
        }
    }
*/
    return 0;
}