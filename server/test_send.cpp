#include "s_send_file.h"
#include <iostream>
#include <algorithm>

using namespace std;
static void int_to_ith_chars(std::string& _buf,int num){
    std::string tmp="";
    while(num!=0){
        tmp+=(char)('0'+num%10);
        num=num/10;
    }
    reverse(tmp.begin(),tmp.end());
    _buf+=tmp;
}
int main(){
    
    s_send_file *ff=new s_send_file("/home/sniper/filetrans_data/test.txt");
    //printf("%d\n",ff->exec_s_send_file_size());
    std::string str="FILE_SIZE:";
    int n=ff->exec_s_send_file_data(str,4096);
    cout<<n<<endl;
    cout<<str<<endl;
    return 0;
}