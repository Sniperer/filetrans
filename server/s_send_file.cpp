#include "s_send_file.h"

#ifdef DEBUG
#define pr_data(str) printf("%s\n",str.c_str());
#else
#define pr_data(str)
#endif

int min(int a,int b){
    return a<b?a:b;
}

s_send_file::s_send_file(std::string _path):path_name(_path){
    fs.open(path_name,std::ios::in|std::ios::out|std::ios::binary|std::ios::ate);    
}

s_send_file::~s_send_file(){
    fs.close();
}

std::string s_send_file::exec_s_send_file_name(){
    return file_name;
}

int s_send_file::exec_s_send_file_size(){
    if(!fs.is_open()){
        return -1;
    }
    fs.seekg(0,std::ios::end);
    file_size=fs.tellg();
    fs.seekg(0);
    return file_size;
}

int s_send_file::exec_s_send_file_data(std::string& _data,size_t len){
    int ans=min(len,exec_s_send_file_size()-send_file_size);
    _data.resize(ans);
    fs.read(&_data[0],ans);
    //std::cout<<_data<<std::endl;
    //_data.resize(ans);
    send_file_size+=ans;
    return ans;
}