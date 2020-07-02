#include "s_recv_file.h"

#ifdef DEBUG
#define pr_data(str) printf("%s\n",str.c_str());
#else 
#define pr_data(str) 
#endif

s_recv_file::s_recv_file(){
    extern int errno;
    char *path=getenv("HOME");
    printf("path=%s\n",path);
    s_save_file_dir=path;
    s_save_file_dir+="/filetrans_data";
    if(opendir(s_save_file_dir.c_str())==NULL&&errno==2){
        if(mkdir(s_save_file_dir.c_str(),S_IRUSR|S_IWUSR|S_IXUSR)<0){
            std::cout<<"some error!"<<std::endl;
        }
    }
}
void s_recv_file::s_recv_file_set_file_name(std::string &&_name){
    file_name=_name;
    std::string file_full_name=s_save_file_dir+"/"+file_name;
    extern int errno;
    int fd;
    if((fd=open(file_full_name.c_str(),O_CREAT|O_WRONLY|O_TRUNC,S_IRUSR|S_IWUSR))<0){
        printf("some error:%s\n",strerror(errno));
        exit(1);
    }
    close(fd);
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
    std::string file_full_name=s_save_file_dir+"/"+file_name;
    pr_data(file_full_name);
    pr_data(file_data);
    //recv_file_size+=file_data.size();
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
    fflush(fs);
    //file_data="";
    fs.close();
}

int s_recv_file::s_get_size_diff(){
    return file_size-recv_file_size;

}

int s_recv_file::s_recv_file_get_file_size(){
    return recv_file_size;
}