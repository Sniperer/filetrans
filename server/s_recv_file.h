#ifndef _S_RECV_FILE_H
#define _S_RECV_FILE_H

#include <string>
#include <fstream>
#include <iostream>

const std::string s_save_file_dir="~/filetrans_data";

class s_recv_file{
    public:
        s_recv_file()=default;
        ~s_recv_file()=default;
        void s_recv_file_set_file_name(std::string &&_name);
        void s_recv_file_set_file_data(std::string &&_data);
        void s_recv_file_set_file_size(size_t _size);
        int  s_recv_file_get_file_size();
        void s_recv_file_size(size_t _size);
        int  s_get_size_diff();
        void s_mk_file();
    private:
        std::string file_name;
        std::string file_data;
        size_t file_size;
        size_t recv_file_size=0;        
};

#endif