#include <iostream>
using namespace std;

class text
{
public:
    int a, b, c;
    text()
    {
        a = 0;
        b = 0;
        c = 0;
    }
    text& change(text& origin)
    {
        origin.a = 100;
        origin.b = 1000;
        origin.c = 10000;
        return origin;
    }
};

int main()
{
    text a, b;
    a.a = 1;
    a.b = 2;
    a.c = 3;
    a = a.change(b);
    cout << a.a <<" "<< a.b <<" "<<a.c<< endl;
    return 0;
}