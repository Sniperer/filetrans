#ifndef _S_RECV_FILE_H
#define _S_RECV_FILE_H

#include <string>
#include <fstream>
#include <iostream>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

class c_recv_file{
    public:
        c_recv_file(std::string &&_fname);
        ~c_recv_file()=default;
        void c_recv_file_set_file_name(std::string &&_name);
        void c_recv_file_set_file_data(std::string &&_data);
        void c_recv_file_set_file_size(size_t _size);
        int  c_recv_file_get_file_size();
        std::string c_recv_file_get_s_file_name();
        void c_recv_file_size(size_t _size);
        int  c_get_size_diff();
        void c_mk_file();
    private:
        std::string c_save_file_dir;
        std::string s_file_name;
        std::string file_name;
        std::string file_data;
        size_t file_size;
        size_t recv_file_size=0;    

};

#endif
