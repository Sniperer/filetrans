#ifndef _C_SEND_FILE_H
#define _C_SEND_FILE_H

#include <string>
#include <fstream>
#include <iostream>

class c_send_file{
    public:
        c_send_file(std::string _path,std::string _name);
        ~c_send_file();
        int exec_c_send_file_size();
        std::string exec_c_send_file_name();
        int exec_c_send_file_data(std::string& _data,size_t len);
    private:
        std::string path_name;
        std::string file_name;
        std::string file_data;
        std::fstream fs;
        size_t file_size;
        size_t send_file_size=0;
};
#endif