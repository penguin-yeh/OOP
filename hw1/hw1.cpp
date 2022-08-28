#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
using namespace std;  

class packet
{
public:
    int src;
    int des;
    int last_hop;
    int next_hop;
};

class node
{
public:
    int id;
    double x_axis;
    double y_axis;
    vector<node*> neighbors;
    queue<packet> pack;
};

int checkQueue(node &a)
{
    if(a.pack.size()!=0)
    {
        return 1;
    }  
    return 0;
}

double dis(node &a,node &b)
{
    return sqrt((a.x_axis-b.x_axis)*(a.x_axis-b.x_axis)+(a.y_axis-b.y_axis)*(a.y_axis-b.y_axis));
}

packet* getNextHop(vector<node> &info,packet *p,int j)
{
    double min = dis(info[j],info[p->des]);
    int flag = 0;
    for(int i=0;i<info[j].neighbors.size();i++)
    {
        if(dis(info[info[j].neighbors[i]->id],info[p->des])<min)
        {
            flag = 1;
            min = dis(info[info[j].neighbors[i]->id],info[p->des]);
            p->last_hop=j;
            p->next_hop=info[j].neighbors[i]->id;
        }
    }
    if(flag==1)
    {
        return p;
    }
    return nullptr;//dead end
}

void send(vector<node> &info,packet *p)
{
    for(int i=0;i<info[p->last_hop].neighbors.size();i++)
    {
        info[info[p->last_hop].neighbors[i]->id].pack.push(*p);
    }
}

int main()
{
    int node_cnt;
    int pair_cnt;
    int cur;
    double x;
    double y;
    node *n;
    n = new node;
    vector<node> node_info;
    //read nodes' information
    cin >> node_cnt;
    for(int i=0;i<node_cnt;i++)
    {
        cin >> cur;
        cin >> x;
        cin >> y;
        n->id=cur;
        n->x_axis=x;
        n->y_axis=y;
        node_info.push_back(*n);
    }
    //find each node's neighbors
    for(int i=0;i<node_cnt;i++)
    {
        for(int j=0;j<node_cnt;j++)
        {
            if(i==j)
                continue;
            if(dis(node_info[i],node_info[j])<=1)
            {
                node_info[i].neighbors.push_back(&node_info[j]);
            }
        }
    }
    //read pairs
    cin >> pair_cnt;
    cout << pair_cnt << endl;
    for(int i=0;i<pair_cnt;i++)
    {
        packet *p;
        p = new packet;
        for(int j=0;j<node_info.size();j++)
        {
            while(!node_info[j].pack.empty())
            {
                node_info[j].pack.pop();
            }
        }
        cin >> p->src;
        cin >> p->des;
        p->last_hop = -1;
        p->next_hop = p->src;//at first,no last_hop;next is itself
        node_info[p->src].pack.push(*p);
        //check every nodes' queue
        while(p!=nullptr && p->next_hop!=p->des)
        {
            // cout << "last:" << p->last_hop << " next:" << p->next_hop << endl;
            // cout << "des:" << p->des << endl;
            for(int j=0;j<node_cnt;j++)
            {
                if(checkQueue(node_info[j])==1)
                {
                    *p = node_info[j].pack.front();//pop_front
                    node_info[j].pack.pop();
                    // cout << "last:" << p->last_hop << " next:" << p->next_hop << endl;
                    // cout << "des:" << p->des << endl;
                    if(p->next_hop==j && p->des!=j)
                    {
                        cout << j << " ";
                        p = getNextHop(node_info,p,j);//problem!!!!!!!
                        // cout << "last:" << p->last_hop << " next:" << p->next_hop << endl;
                        // cout << "des:" << p->des << endl;
                        if(p == nullptr) 
                        {
                            //cout << "nullptr" << endl;
                            cout << endl;
                            break;
                        }
                        if(p->next_hop == p->des)
                        {
                            //cout << "end" << endl;
                            cout << p->des << endl;
                            break;
                        }
                        send(node_info,p);
                        {
                            //cout << "go" << endl;
                            break;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
