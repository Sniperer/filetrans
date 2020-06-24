#include <iostream>
#include "c_send_file.h"
#include "c_socket_connect.h"

int main(int argc,char* argv[]){
    if(argc==1){
        std::cout<<"please input file path."<<std::endl;
        return 0;
    }
    else{
    //    c_socket_connect c_con(8848);
        for(int i=1;i<argc;i++){
            std::cout<<"file path:";
            std::cout<<argv[i]<<std::endl;
            c_send_file *fd=new c_send_file(argv[i]);
            c_socket_connect *c_con=new c_socket_connect(8848,fd);
            c_con->exec_c_connect();
        }
    }

    return 0;
}