#include <iostream>
#include <string>
#include "c_send_file.h"
#include "c_socket_connect.h"

int main(int argc,char* argv[]){
    std::string ip_addr,username,password,srcpath,dstpath;
    std::cout<<">filetrans start.\n"<<std::endl;
    std::cout<<">input ip address:";
    std::cin>>ip_addr;
    std::cout<<">input username:";
    std::cin>>username;
    std::cout<<">input password:";
    std::cin>>password;
    std::cout<<">input cilent filepath:";
    std::cin>>srcpath;
    std::cout<<">input server filepath:";
    std::cin>>dstpath;
    c_send_file *fd=new c_send_file(srcpath,dstpath);
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