#include <iostream>
#include <vector>
using namespace std;

class node
{
public:
    int id;
    double x_axis;
    double y_axis;
    vector<node*> neighbors;
};

class packet
{
public:
    int src;
    int des;
    int last_hop;
    int next_hop;
};


int main()
{
    for(int i=0;i<100;i++)
    {
        packet *p;
    p = new packet;
    p->next_hop = 123456;
    p = nullptr;
    p = new packet;
    p->next_hop = 654321;
    cout << p->next_hop <<endl;
    cout << i << endl;
    }
    return 0;
}