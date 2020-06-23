#include "s_recv_file.h"

void s_recv_file::s_recv_file_set_file_name(std::string &&_name){
    file_name=_name;
}

void s_recv_file::s_recv_file_set_file_data(std::string &&_data){
    file_data=_data;
    s_mk_file();
}

void s_recv_file::s_recv_file_set_file_size(size_t _size){
    file_size=_size;
}

void s_recv_file::s_recv_file_size(size_t _size){
    recv_file_size+=_size;
}

void s_recv_file::s_mk_file(){
    std::string file_full_name=s_save_file_dir+file_name;
    recv_file_size+=file_data.size();
    std::fstream fs;
    fs.open(file_full_name,std::ios::in|std::ios::out|std::ios::binary|std::ios::ate);
    if(!fs.is_open()){
        fs.clear();
        fs.open(file_full_name,std::ios::out);
        fs.close();
        fs.open(file_full_name,std::ios::in|std::ios::out|std::ios::binary|std::ios::ate);
    }
    fs<<file_data;
    file_data="";
    fs.close();
}

int s_recv_file::s_get_size_diff(){
    return file_size-recv_file_size;

}

int s_recv_file::s_recv_file_get_file_size(){
    return recv_file_size;
}