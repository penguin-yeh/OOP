#include <iostream>
using namespace std;

template <class T>
void show(int a, T b, T c)
{
    cout << a << endl << b << endl << c << endl;
}

int main()
{
    show(10,5.5,8.7);
    return 0;
}