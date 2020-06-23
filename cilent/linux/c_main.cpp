#include <iostream>

int main(int argc,char* argv[]){
    if(argc==1){
        std::cout<<"please input file path."<<std::endl;
        return 0;
    }
    else{
        for(int i=1;i<argc;i++){
            std::cout<<"file path:";
            std::cout<<argv[i]<<std::endl;
        }
    }
    return 0;
}