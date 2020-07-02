#include "c_send_file.h"

#ifdef DEBUG
#define pr_data(str) printf("%s\n",str.c_str());
#else 
#define pr_data(str) 
#endif 

int min(int a,int b){
    return a<b?a:b;
}

c_send_file::c_send_file(std::string _path,std::string _name):path_name(_path),file_name(_name){
#ifdef DEBUG
    std::cout<<path_name<<" "<<file_name<<std::endl;
#endif
    fs.open(path_name,std::ios::in|std::ios::out|std::ios::binary|std::ios::ate);
}

c_send_file::~c_send_file(){
    fs.close();
}

std::string c_send_file::exec_c_send_file_name(){
    return file_name;
}

int c_send_file::exec_c_send_file_size(){
    if(!fs.is_open()){
#ifdef DEBUG
    std::cout<<"file not exist."<<std::endl;
#endif
        return 1;
    }
    file_size=fs.tellg();
#ifdef DEBUG1
    std::cout<<"[c_send_file]file_size:"<<file_size<<std::endl;
#endif
    fs.seekg(0);
    return file_size;
}

int c_send_file::exec_c_send_file_data(std::string& _data,size_t len){
    int ans=min(len,file_size-send_file_size);
    //_data[ans]='\0';
    fs.read(&_data[0],ans);
    //pr_data(_data);
    send_file_size+=ans;
    return ans;
}
