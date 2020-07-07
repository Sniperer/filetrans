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
    while(1){
        std::cout<<">input function[1/2/3/4](upload file/download file/show files):";
        std::cin>>op;
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
            c_recv_file *fd=new c_recv_file(std::move(srcpath));
            fd->c_recv_file_set_file_name(std::move(dstpath));
            c_con->exec_c_connect(fd);
        }
        else if(op==3){
            /*show files*/
            std::cout<<"show files:\n"<<std::endl;
            c_con->exec_c_connect();
        }
        else if(op==4){
            return 0;
        }
        else{
            std::cout<<"?"<<std::endl;
        }
    }
    return 0;
}