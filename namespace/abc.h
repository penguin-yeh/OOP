#ifndef abc_h
#define abc_h

#include <iostream>
using namespace std;
namespace
{
    void test()
    {
        cout << "HIHIHIH" << endl;
    }
}

namespace andy
{
    class HI
    {
        public:
            int H = 10;
            void f();
            HI(int a) {H = a;} 
            HI() {H = 0;}
    };

}


#endif