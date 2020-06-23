#include <iostream>

using namespace std;

class mstring{
    public:
        mstring(const char* pt){
            cout<<"constructor call."<<endl;
            buf=pt;
        }
        ~mstring(){
            
        }
    private:
        const char* buf;
};
mstring b("456");
void _swap(mstring && _a){
    b=_a;
}
int main(){
    mstring a("123");
    _swap(std::move(a));    
//    cout<<b<<endl;
    return 0;
}