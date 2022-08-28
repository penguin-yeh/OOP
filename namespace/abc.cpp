#include "abc.h"

// namespace andy
// {
//     class HIHI : public HI
//     {
//         public:
//             void f()
//             {
//                 cout << HI << endl;
//             }
//     };
// }
void andy::HI::f()
{
    cout << H << endl;
}

using andy::HI;

int main()
{
    HI a;
    a.f();
    return 0;
}