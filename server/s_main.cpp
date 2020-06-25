#include "threadpool.h"
#include "s_socket_listen.h"
#include <iostream>


int main(){
    threadpool *tp=new threadpool(4);
    thread_task* task=new s_socket_listen_task(tp);
    tp->add_task(task);
    while(1){
        
    }
    return 0;
}