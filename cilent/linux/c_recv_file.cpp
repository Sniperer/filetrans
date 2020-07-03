#include "c_recv_file.h"

#ifdef DEBUG
#define pr_data(str) printf("%s\n",str.c_str());
#else
#define pr_data(str)
#endif

c_recv_file::c_recv_file(){
    extern int errno;
    char *path=getenv("HOME");
    printf("path=%s\n",path);
    c_save_file_dir=path;
    c_save_file_dir+="/filetrans_data_c";
    if(!opendir(c_save_file_dir.c_str())==NULL&&errno==2){
        if(mkdir(c_save_file_dir.c_str(),S_IRUSR|S_IWUSR|S_IXUSR)<0){
            std::cout<<"some error!"<<std::endl;
        }
    }
}

void c_recv_file::c_recv_file_set_file_name(std::string &&_name){
    file_name=_name;
    std::string file_full_name=c_save_file_dir+"/"+file_name;
    extern int errno;
    int fd;
    if((fd=open(file_full_name.c_str(),O_CREAT|O_WRONLY|O_TRUNC,S_IRUSR|S_IWUSR))<0){
        printf("some error:%s\n",strerror(errno));
        exit(1);
    }
    close(fd);
}

void c_recv_file::c_recv_file_set_file_data(std::string &&_data){
    file_data=_data;
    c_mk_file();
}

void c_recv_file::c_recv_file_set_file_size(size_t _size){
    file_size=_size;
}

void c_recv_file::c_recv_file_size(size_t _size){
    recv_file_size+=_size;
}

void c_recv_file::c_mk_file(){
    std::string file_full_name=c_save_file_dir+"/"+file_name;
    pr_data(file_full_name);
    pr_data(file_data);
    std::fstream fs;
    fs.open(file_full_name,std::ios::in|std::ios::out|std::ios::binary|std::ios::ate);
    if(!fs.is_open()){
#ifdef DEBUG
    std::cout<<"file can't open"<<std::endl;
#endif
        fs.clear();
        fs.open(file_full_name,std::ios::out);
        fs.close();
        fs.open(file_full_name,std::ios::in|std::ios::out|std::ios::binary|std::ios::ate);
    }
    fs<<file_data;
    fs.close();
}

int c_recv_file::c_get_size_diff(){
    return file_size-recv_file_size;
}

int c_recv_file::c_recv_file_get_file_size(){
    return recv_file_size;
}