#include <s_recv_file.h>

int s_recv_file::s_recv_file_set_file_name(std::string &&_name){
    file_name=_name;
}

int s_recv_file::s_recv_file_set_file_data(std::string &&_data){
    file_data=_data;
}

int s_recv_file::s_recv_file_size(size_t _size){
    file_size=_size;
}