#ifndef _S_SEND_FILE_H
#define _S_SEND_FILE_H

#include <string>
#include <fstream>
#include <iostream>

class s_send_file{
    public:
        s_send_file(std::string _path);
        ~s_send_file();
        int exec_s_send_file_size();
        std::string exec_s_send_file_name();
        int exec_s_send_file_data(std::string& _data,size_t len);
    private:
        std::string path_name;
        std::string file_name;
        std::string file_data;
        std::fstream fs;
        size_t file_size;
        size_t send_file_size=0;
};

#endif