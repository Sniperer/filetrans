#ifndef _S_RECV_FILE_H
#define _S_RECV_FILE_H

#include <string>
#include <fstream>
#include <iostream>

class s_recv_file{
    public:
        s_recv_file();
        ~s_recv_file();
        int s_recv_file_set_file_name(std::string &&_name);
        int s_recv_file_set_file_data(std::string &&_data);
        int s_recv_file_size(size_t _size);
        int s_mk_file();
    private:
        std::string file_name;
        std::string file_data;
        size_t file_size;        
}

#endif