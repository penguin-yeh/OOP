#include <iostream>
using namespace std;

class Money
{
    public:
        Money(): dollar(0) { count++;}
        Money(int a): dollar(a) { count++;}
        Money(const Money &a) { dollar = a.dollar; count++;}
        ~Money() { count--; }
        Money& hihi() {cout << "!!!" << endl; return *this;}
        Money& operator =(const Money &a)
        {
            dollar = a.dollar;
            return *this;
        }
        const friend  Money operator + (const Money &a, const Money &b);
        const friend  Money operator - (const Money &a, const Money &b);
        friend istream& operator >> (istream& in, Money &a);
        friend ostream& operator << (ostream& out,Money &b); 
        static int count ;
        int  getDollar()
        {
            return dollar;
        }
        void f()
        {
            cout << dollar << endl;
        }
    private:
        int dollar;
};

const Money operator + (const Money &a,const Money &b)
{
    return Money(a.dollar + b.dollar);
}
const Money operator - (const Money &a,const Money &b)
{
    return Money(a.dollar - b.dollar);
}

const Money operator - (Money &a)
{
    return Money(-a.getDollar());
}

istream& operator >> (istream& in, Money &a)
{
    in >> a.dollar;
    return in;
}

ostream& operator << (ostream& out, Money &b)
{
    out << b.dollar << endl;
    out << "hihi" << endl;
    return out;
}

int Money::count = 0;

int main()
{
    Money l(50);
    int r =10;
    Money sum = l + r;
    sum.getDollar();
    cout << "count " << Money::count << endl;
    sum = r + l;
    sum.getDollar();
    //sum = r + l;
    cout << 10+20 << endl;
    Money hi;
    cin >> hi;
    cout << hi;
    Money h2;
    h2 = -hi;
    cout << "h2 = " << h2 ;
    return 0;
}
