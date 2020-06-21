#include "threadpool.h"
#include <iostream>

class print:public thread_task{
    public:
        int nw=2;
        virtual int exec(){
            std::cout<<nw<<std::endl;
            nw++;
        }
};

int main(){
    threadpool *tp=new threadpool(4);
    print *ta=new print();
    for(int i=1;i<20;i++){
        tp->add_task(ta);
    }    
    return 0;
}