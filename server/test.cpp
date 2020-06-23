#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(){
    string filename1="test.txt";
    string filename2="test2.txt";

    std::fstream _f123s;
    _f123s.open(filename1,std::ios::in|std::ios::out|std::ios::binary|std::ios::ate);
    //char buf[100];
    //_f123s.put(buf);
    auto size=_f123s.tellg();
    std::string str(size,'\0');
    std::cout<<size<<std::endl;
    _f123s.seekg(0);
    if(_f123s.read(&str[0],size))
        std::cout<<str<<std::endl;


    std::fstream _f123s2;
    _f123s2.open(filename2,std::ios::out);
    _f123s2.close();
    _f123s2.open(filename2,std::ios::out|std::ios::in|std::ios::binary);
    _f123s2<<str<<std::endl;
    _f123s.close();
    _f123s2.close();

    return 0;  
}