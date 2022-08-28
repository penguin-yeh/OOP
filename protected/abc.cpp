#include <iostream>
using namespace std;

class D
{   protected:
        int data;
    public:
        void f()
        {
            cout << data << endl;
        }
        D() {data = 100;}
};

class B:public D
{    
    public:
        void f()
        {
            cout << data << "HI" << endl;
        }
};

int main()
{
    D obj_d;
    B obj_b;
    obj_d.f();
    //obj_b.data = 10;  error!!
    return 0;
}
