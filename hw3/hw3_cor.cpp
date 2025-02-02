#include <iostream>
#include <map>
#include <queue>
#include <utility>
#include <climits>
#include <functional>
#include <iomanip>
#include <stack>

using namespace std;

#define SET(func_name,type,var_name,_var_name) void func_name(type _var_name) { var_name = _var_name ;} 
#define GET(func_name,type,var_name) type func_name() const { return var_name ;}

class header;
class payload;
class packet;
class node;
class event;
class link; // new

// for simplicity, we use a const int to simulate the delay
// if you want to simulate the more details, you should revise it to be a class
const unsigned int ONE_HOP_DELAY = 10;
const unsigned int BROCAST_ID = UINT_MAX;
// BROCAST_ID means that all neighbors are receivers; UINT_MAX is the maximum value of unsigned int

class header {
    public:
        virtual ~header() {}

        SET(setSrcID, unsigned int , srcID, _srcID);
        SET(setDstID, unsigned int , dstID, _dstID);
        SET(setPreID, unsigned int , preID, _preID);
        SET(setNexID, unsigned int , nexID, _nexID);
        GET(getSrcID, unsigned int , srcID);
        GET(getDstID, unsigned int , dstID);
        GET(getPreID, unsigned int , preID);
        GET(getNexID, unsigned int , nexID);
        
        SET(setDstInfo, int , data, _data);
        GET(getDstInfo, int , data);
        virtual string type() = 0;
        
        // factory concept: generate a header
        class header_generator {
                // lock the copy constructor
                header_generator(header_generator &){}
                // store all possible types of header
                static map<string,header_generator*> prototypes;
            protected:
                // allow derived class to use it
                header_generator() {}
                // after you create a new header type, please register the factory of this header type by this function
                void register_header_type(header_generator *h) { prototypes[h->type()] = h; }
                // you have to implement your own generate() to generate your header
                virtual header* generate() = 0 ;
            public:
                // you have to implement your own type() to return your header type
        	    virtual string type() = 0 ;
        	    // this function is used to generate any type of header derived
        	    static header * generate (string type) {
            		if(prototypes.find(type) != prototypes.end()){ // if this type derived exists 
            			return prototypes[type]->generate(); // generate it!!
            		}
            		std::cerr << "no such header type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static void print () {
            	    cout << "registered header types: " << endl;
            	    for (map<string,header::header_generator*>::iterator it = prototypes.begin(); it != prototypes.end(); it ++)
            	        cout << it->second->type() << endl;
            	}
            	virtual ~header_generator(){};
        };
        
    protected:
        header():srcID(BROCAST_ID),dstID(BROCAST_ID),preID(BROCAST_ID),nexID(BROCAST_ID){} // this constructor cannot be directly called by users

    private:
        unsigned int srcID;
        unsigned int dstID;
        unsigned int preID;
        unsigned int nexID;
        int data;
        header(header&){} // this constructor cannot be directly called by users
};
map<string,header::header_generator*> header::header_generator::prototypes;
//==============================header==================================
class GR_header : public header{
        double dstX; // the position of the destination
        double dstY;
		int dstID;
        GR_header(GR_header&){} // cannot be called by users
        
    protected:
        GR_header(): dstX(0), dstY(0) {} // this constructor cannot be directly called by users

    public:
        ~GR_header(){}
        
        SET(setDstX,double,dstX,_dstX);
        GET(getDstX,double,dstX);
        SET(setDstY,double,dstY,_dstY);
        GET(getDstY,double,dstY);

        //SET(setDstInfo, int,dstID,_dstID);
        //GET(getDstInfo, int,dstID);
        
        string type() { return "GR_header"; }

        class GR_header_generator;
        friend class GR_header_generator;
        // GR_header_generator is derived from header_generator to generate a header
        class GR_header_generator : public header_generator{
                static GR_header_generator sample;
                // this constructor is only for sample to register this header type
                GR_header_generator() { /*cout << "GR_header registered" << endl;*/ register_header_type(&sample); }
            protected:
                virtual header * generate(){
                    // cout << "GR_header generated" << endl;
                    return new GR_header; 
                }
            public:
                virtual string type() { return "GR_header";}
                ~GR_header_generator(){}
        
        };
};
GR_header::GR_header_generator GR_header::GR_header_generator::sample;

// ccu
class Hello_header : public header{
        double dstX; // the position of the destination
        double dstY;
		int dstID;
        Hello_header(Hello_header&){} // cannot be called by users
        
    protected:
        Hello_header(): dstX(0), dstY(0) {} // this constructor cannot be directly called by users

    public:
        ~Hello_header(){}
        
        SET(setDstX,double,dstX,_dstX);
        GET(getDstX,double,dstX);
        SET(setDstY,double,dstY,_dstY);
        GET(getDstY,double,dstY);

        string type() { return "Hello_header"; }

        class Hello_header_generator;
        friend class Hello_header_generator;
        // Hello_header_generator is derived from header_generator to generate a header
        class Hello_header_generator : public header_generator{
                static Hello_header_generator sample;
                // this constructor is only for sample to register this header type
                Hello_header_generator() { /*cout << "Hello_header registered" << endl;*/ register_header_type(&sample); }
            protected:
                virtual header * generate(){
                    // cout << "Hello_header generated" << endl;
                    return new Hello_header; 
                }
            public:
                virtual string type() { return "Hello_header";}
                ~Hello_header_generator(){}
        
        };
};
Hello_header::Hello_header_generator Hello_header::Hello_header_generator::sample;


//===================================================================================
class payload {
        payload(payload&){} // this constructor cannot be directly called by users
    protected:
        payload(){}
    public:
        virtual ~payload(){}
        virtual string type() = 0;
        
        class payload_generator {
                // lock the copy constructor
                payload_generator(payload_generator &){}
                // store all possible types of header
                static map<string,payload_generator*> prototypes;
            protected:
                // allow derived class to use it
                payload_generator() {}
                // after you create a new payload type, please register the factory of this payload type by this function
                void register_payload_type(payload_generator *h) { prototypes[h->type()] = h; }
                // you have to implement your own generate() to generate your payload
                virtual payload* generate() = 0;
            public:
                // you have to implement your own type() to return your header type
        	    virtual string type() = 0;
        	    // this function is used to generate any type of header derived
        	    static payload * generate (string type) {
            		if(prototypes.find(type) != prototypes.end()){ // if this type derived exists 
            			return prototypes[type]->generate(); // generate it!!
            		}
            		std::cerr << "no such payload type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static void print () {
            	    cout << "registered payload types: " << endl;
            	    for (map<string,payload::payload_generator*>::iterator it = prototypes.begin(); it != prototypes.end(); it ++)
            	        cout << it->second->type() << endl;
            	}
            	virtual ~payload_generator(){};
        };
};
map<string,payload::payload_generator*> payload::payload_generator::prototypes;

//====================================payload===================================
class GR_payload : public payload {
        GR_payload(GR_payload&){}
        
        string msg;
        
    protected:
        GR_payload(){} // this constructor cannot be directly called by users
    public:
        ~GR_payload(){}
        
        SET(setMsg,string,msg,_msg);
        GET(getMsg,string,msg);
        
        string type() { return "GR_payload"; }
        
        class GR_payload_generator;
        friend class GR_payload_generator;
        // GR_payload is derived from payload_generator to generate a payload
        class GR_payload_generator : public payload_generator{
                static GR_payload_generator sample;
                // this constructor is only for sample to register this payload type
                GR_payload_generator() { /*cout << "GR_payload registered" << endl;*/ register_payload_type(&sample); }
            protected:
                virtual payload * generate(){ 
                    // cout << "GR_payload generated" << endl;
                    return new GR_payload; 
                }
            public:
                virtual string type() { return "GR_payload";}
                ~GR_payload_generator(){}
        };
};
GR_payload::GR_payload_generator GR_payload::GR_payload_generator::sample;
//ccu
class Hello_payload : public payload {
        Hello_payload(Hello_payload&){}
        
        string msg;
        
    protected:
        Hello_payload(){} // this constructor cannot be directly called by users
    public:
        ~Hello_payload(){}
        
        SET(setMsg,string,msg,_msg);
        GET(getMsg,string,msg);
        
        string type() { return "Hello_payload"; }
        
        class Hello_payload_generator;
        friend class Hello_payload_generator;
        // Hello_payload is derived from payload_generator to generate a payload
        class Hello_payload_generator : public payload_generator{
                static Hello_payload_generator sample;
                // this constructor is only for sample to register this payload type
                Hello_payload_generator() { /*cout << "Hello_payload registered" << endl;*/ register_payload_type(&sample); }
            protected:
                virtual payload * generate(){ 
                    // cout << "Hello_payload generated" << endl;
                    return new Hello_payload; 
                }
            public:
                virtual string type() { return "Hello_payload";}
                ~Hello_payload_generator(){}
        };
};
Hello_payload::Hello_payload_generator Hello_payload::Hello_payload_generator::sample;


//=====================================================================

class packet{
        // a packet usually contains a header and a payload
        header *hdr;
        payload *pld;
        unsigned int p_id;
        static unsigned int last_packet_id ; 
        
        packet(packet &) {}
        static int live_packet_num ;
    protected:
        // these constructors cannot be directly called by users
        packet(): hdr(nullptr), pld(nullptr) { p_id=last_packet_id++; live_packet_num ++; }
        packet(string _hdr, string _pld, bool rep = false, unsigned int rep_id = 0) {
            if (! rep ) // a duplicated packet does not have a new packet id
                p_id = last_packet_id ++;
            else
                p_id = rep_id;
            hdr = header::header_generator::generate(_hdr); 
            pld = payload::payload_generator::generate(_pld); 
            live_packet_num ++;
        }
    public:
        virtual ~packet(){ 
            // cout << "packet destructor begin" << endl;
            if (hdr != nullptr) 
                delete hdr; 
            if (pld != nullptr) 
                delete pld; 
            live_packet_num --;
            // cout << "packet destructor end" << endl;
        }
        
        SET(setHeader,header*,hdr,_hdr);
        GET(getHeader,header*,hdr);
        SET(setPayload,payload*,pld,_pld);
        GET(getPayload,payload*,pld);
        GET(getPacketID,unsigned int,p_id);
        
        static void discard ( packet* &p ) {
            // cout << "checking" << endl;
            if (p != nullptr) {
                // cout << "discarding" << endl;
                // cout << p->type() << endl;
                delete p;
                // cout << "discarded" << endl;
            }
            p = nullptr;
            // cout << "checked" << endl;
        }
        virtual string type () = 0;
        
        static int getLivePacketNum () { return live_packet_num; }
        
        class packet_generator;
        friend class packet_generator;
        class packet_generator {
                // lock the copy constructor
                packet_generator(packet_generator &){}
                // store all possible types of packet
                static map<string,packet_generator*> prototypes;
            protected:
                // allow derived class to use it
                packet_generator() {}
                // after you create a new packet type, please register the factory of this payload type by this function
                void register_packet_type(packet_generator *h) { prototypes[h->type()] = h; }
                // you have to implement your own generate() to generate your payload
                virtual packet* generate ( packet *p = nullptr) = 0;
            public:
                // you have to implement your own type() to return your packet type
        	    virtual string type() = 0;
        	    // this function is used to generate any type of packet derived
        	    static packet * generate (string type) {
            		if(prototypes.find(type) != prototypes.end()){ // if this type derived exists 
            			return prototypes[type]->generate(); // generate it!!
            		}
            		std::cerr << "no such packet type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static packet * replicate (packet *p) {
            	    if(prototypes.find(p->type()) != prototypes.end()){ // if this type derived exists 
            			return prototypes[p->type()]->generate(p); // generate it!!
            		}
            		std::cerr << "no such packet type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static void print () {
            	    cout << "registered packet types: " << endl;
            	    for (map<string,packet::packet_generator*>::iterator it = prototypes.begin(); it != prototypes.end(); it ++)
            	        cout << it->second->type() << endl;
            	}
            	virtual ~packet_generator(){};
        };
};
map<string,packet::packet_generator*> packet::packet_generator::prototypes;
unsigned int packet::last_packet_id = 0 ;
int packet::live_packet_num = 0;


// this packet is used to tell the destination the msg
class GR_packet: public packet {
        GR_packet(GR_packet &) {}
        
    protected:
        GR_packet(){} // this constructor cannot be directly called by users
        GR_packet(packet*p): packet(p->getHeader()->type(), p->getPayload()->type(), true, p->getPacketID()) {
            *(dynamic_cast<GR_header*>(this->getHeader())) = *(dynamic_cast<GR_header*> (p->getHeader()));
            *(dynamic_cast<GR_payload*>(this->getPayload())) = *(dynamic_cast<GR_payload*> (p->getPayload()));
            //DFS_path = (dynamic_cast<GR_header*>(p))->DFS_path;
            //isVisited = (dynamic_cast<GR_header*>(p))->isVisited;
        } // for duplicate
        GR_packet(string _h, string _p): packet(_h,_p) {}
        
    public:
        virtual ~GR_packet(){}
        string type() { return "GR_packet"; }
        
        class GR_packet_generator;
        friend class GR_packet_generator;
        // GR_packet is derived from packet_generator to generate a pub packet
        class GR_packet_generator : public packet_generator{
                static GR_packet_generator sample;
                // this constructor is only for sample to register this packet type
                GR_packet_generator() { /*cout << "GR_packet registered" << endl;*/ register_packet_type(&sample); }
            protected:
                virtual packet *generate (packet *p = nullptr){
                    // cout << "GR_packet generated" << endl;
                    if ( nullptr == p )
                        return new GR_packet("GR_header","GR_payload"); 
                    else
                        return new GR_packet(p); // duplicate
                }
            public:
                virtual string type() { return "GR_packet";}
                ~GR_packet_generator(){}
        };
};
GR_packet::GR_packet_generator GR_packet::GR_packet_generator::sample;
//ccu
class Hello_packet: public packet {
        Hello_packet(Hello_packet &) {}
        
    protected:
        Hello_packet(){} // this constructor cannot be directly called by users
        Hello_packet(packet*p): packet(p->getHeader()->type(), p->getPayload()->type(), true, p->getPacketID()) {
            *(dynamic_cast<Hello_header*>(this->getHeader())) = *(dynamic_cast<Hello_header*> (p->getHeader()));
            *(dynamic_cast<Hello_payload*>(this->getPayload())) = *(dynamic_cast<Hello_payload*> (p->getPayload()));
            //DFS_path = (dynamic_cast<Hello_header*>(p))->DFS_path;
            //isVisited = (dynamic_cast<Hello_header*>(p))->isVisited;
        } // for duplicate
        Hello_packet(string _h, string _p): packet(_h,_p) {}
        
    public:
        virtual ~Hello_packet(){}
        string type() { return "Hello_packet"; }
        
        class Hello_packet_generator;
        friend class Hello_packet_generator;
        // Hello_packet is derived from packet_generator to generate a pub packet
        class Hello_packet_generator : public packet_generator{
                static Hello_packet_generator sample;
                // this constructor is only for sample to register this packet type
                Hello_packet_generator() { /*cout << "Hello_packet registered" << endl;*/ register_packet_type(&sample); }
            protected:
                virtual packet *generate (packet *p = nullptr){
                    // cout << "Hello_packet generated" << endl;
                    if ( nullptr == p )
                        return new Hello_packet("Hello_header","Hello_payload"); 
                    else
                        return new Hello_packet(p); // duplicate
                }
            public:
                virtual string type() { return "Hello_packet";}
                ~Hello_packet_generator(){}
        };
};
Hello_packet::Hello_packet_generator Hello_packet::Hello_packet_generator::sample;

class node {
        // all nodes created in the program
        static map<unsigned int, node*> id_node_table;
        
        unsigned int id;
        map<unsigned int,bool> phy_neighbors;
        
        // you can use the function to get the node's neigbhors in HW2
        // But !!! In HW 3, you are not allowed to use this function 
        // Please define your own neighbors in GR_node
        // Then, define your own Hellp_packet to inherit packet
        // Broadcast Hellp_packet to let your neighbors know your id
        const map<unsigned int,bool> & getPhyNeighbors () { 
            return phy_neighbors;
        }
        
    protected:
        node(node&){} // this constructor should not be used
        node(){} // this constructor should not be used
        node(unsigned int _id): id(_id) { id_node_table[_id] = this; }
    public:
        virtual ~node() { // erase the node
            id_node_table.erase (id) ;
        }
        
        void add_phy_neighbor (unsigned int _id, string link_type = "simple_link"); // we only add a directed link from id to _id
        void del_phy_neighbor (unsigned int _id); // we only delete a directed link from id to _id
        
        queue<packet> test; // ccu2
        void recv (packet *p) {
            packet *tp = p;
            recv_handler(tp); 
            packet::discard(p); 
        } // the packet will be directly deleted after the handler
        void send (packet *p);
        
        // receive the packet and do something; this is a pure virtual function
        virtual void recv_handler(packet *p) = 0;
        void send_handler(packet *P);
        
        static node * id_to_node (unsigned int _id) { return ((id_node_table.find(_id)!=id_node_table.end()) ? id_node_table[_id]: nullptr) ; }
        GET(getNodeID,unsigned int,id);
        
        static void del_node (unsigned int _id) {
            if (id_node_table.find(_id) != id_node_table.end())
                id_node_table.erase(_id);
        }
        static unsigned int getNodeNum () { return id_node_table.size(); }

        class node_generator {
                // lock the copy constructor
                node_generator(node_generator &){}
                // store all possible types of node
                static map<string,node_generator*> prototypes;
            protected:
                // allow derived class to use it
                node_generator() {}
                // after you create a new node type, please register the factory of this node type by this function
                void register_node_type(node_generator *h) { prototypes[h->type()] = h; }
                // you have to implement your own generate() to generate your node
                virtual node* generate(unsigned int _id) = 0;
            public:
                // you have to implement your own type() to return your node type
        	    virtual string type() = 0;
        	    // this function is used to generate any type of node derived
        	    static node * generate (string type, unsigned int _id) {
        	        if(id_node_table.find(_id)!=id_node_table.end()){
        	            std::cerr << "duplicate node id" << std::endl; // node id is duplicated
        	            return nullptr;
        	        }
        	        else if ( BROCAST_ID == _id ) {
        	            cerr << "BROCAST_ID cannot be used" << endl;
        	            return nullptr;
        	        }
            		else if(prototypes.find(type) != prototypes.end()){ // if this type derived exists 
            		    node * created_node = prototypes[type]->generate(_id);
            			return created_node; // generate it!!
            		}
            		std::cerr << "no such node type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static void print () {
            	    cout << "registered node types: " << endl;
            	    for (map<string,node::node_generator*>::iterator it = prototypes.begin(); it != prototypes.end(); it ++)
            	        cout << it->second->type() << endl;
            	}
            	virtual ~node_generator(){};
        };
};
map<string,node::node_generator*> node::node_generator::prototypes;
map<unsigned int,node*> node::id_node_table;


class mycomp {
    bool reverse;
    
    public:
        mycomp(const bool& revparam = false) { reverse=revparam ; }
        bool operator() (const event* lhs, const event* rhs) const;
};

class event {
        event(event*&){} // this constructor cannot be directly called by users
        static priority_queue < event*, vector < event* >, mycomp > events;
        static unsigned int cur_time; // timer
        static unsigned int end_time;
        
        unsigned int trigger_time;
        
        // get the next event
        static event * get_next_event() ;
        static void add_event (event *e) { events.push(e); }
        static hash<string> event_seq;
        
    protected:
        event(){} // it should not be used
        event(unsigned int _trigger_time): trigger_time(_trigger_time) {}
    public:
        virtual void trigger()=0;
        virtual ~event(){}

        virtual unsigned int event_priority() const = 0;
        unsigned int get_hash_value(string string_for_hash) const {
            unsigned int priority = event_seq (string_for_hash);
            return priority;
        }
        
        static void flush_events (); // only for debug
        
        GET(getTriggerTime,unsigned int,trigger_time);
        
        static void start_simulate( unsigned int _end_time ); // the function is used to start the simulation
        
        static unsigned int getCurTime() { return cur_time ; }
        static void getCurTime(unsigned int _cur_time) { cur_time = _cur_time; } 
        // static unsigned int getEndTime() { return end_time ; }
        // static void getEndTime(unsigned int _end_time) { end_time = _end_time; }
        
        virtual void print () const = 0; // the function is used to print the event information

        class event_generator{
                // lock the copy constructor
                event_generator(event_generator &){}
                // store all possible types of event
                static map<string,event_generator*> prototypes;
            protected:
                // allow derived class to use it
                event_generator() {}
                // after you create a new event type, please register the factory of this event type by this function
                void register_event_type(event_generator *h) { prototypes[h->type()] = h; }
                // you have to implement your own generate() to generate your event
                virtual event* generate(unsigned int _trigger_time, void * data) = 0;
            public:
                // you have to implement your own type() to return your event type
        	    virtual string type() = 0;
        	    // this function is used to generate any type of event derived
        	    static event * generate (string type, unsigned int _trigger_time, void * data) {
            		if(prototypes.find(type) != prototypes.end()){ // if this type derived exists
            		    event * e = prototypes[type]->generate(_trigger_time, data);
            		    add_event(e);
            		    return e; // generate it!!
            		}
            		std::cerr << "no such event type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static void print () {
            	    cout << "registered event types: " << endl;
            	    for (map<string,event::event_generator*>::iterator it = prototypes.begin(); it != prototypes.end(); it ++)
            	        cout << it->second->type() << endl;
            	}
            	virtual ~event_generator(){}
        };
};
map<string,event::event_generator*> event::event_generator::prototypes;
priority_queue < event*, vector< event* >, mycomp > event::events;
hash<string> event::event_seq;

unsigned int event::cur_time = 0;
unsigned int event::end_time = 0;

void event::flush_events()
{ 
    cout << "**flush begin" << endl;
    while ( ! events.empty() ) {
        cout << setw(11) << events.top()->trigger_time << ": " << setw(11) << events.top()->event_priority() << endl;
        delete events.top();
        events.pop();
    }
    cout << "**flush end" << endl;
}
event * event::get_next_event() {
    if(events.empty()) 
        return nullptr; 
    event * e = events.top();
    events.pop(); 
    // cout << events.size() << " events remains" << endl;
    return e; 
}
void event::start_simulate(unsigned int _end_time) {
    if (_end_time<0) {
        cerr << "you should give a possitive value of _end_time" << endl;
        return;
    }
    end_time = _end_time;
    event *e; 
    e = event::get_next_event ();
    while ( e != nullptr && e->trigger_time <= end_time ) {
        if ( cur_time <= e->trigger_time )
            cur_time = e->trigger_time;
        else {
            cerr << "cur_time = " << cur_time << ", event trigger_time = " << e->trigger_time << endl;
            break;
        }

        // cout << "event trigger_time = " << e->trigger_time << endl;
        e->print(); // for log
        // cout << " event begin" << endl;
        e->trigger();
        // cout << " event end" << endl;
        delete e;
        e = event::get_next_event ();
    }
    // cout << "no more event" << endl;
}

bool mycomp::operator() (const event* lhs, const event* rhs) const {
    // cout << lhs->getTriggerTime() << ", " << rhs->getTriggerTime() << endl;
    // cout << lhs->type() << ", " << rhs->type() << endl;
    unsigned int lhs_pri = lhs->event_priority();
    unsigned int rhs_pri = rhs->event_priority();
    // cout << "lhs hash = " << lhs_pri << endl;
    // cout << "rhs hash = " << rhs_pri << endl;
    
    if (reverse) 
        return ((lhs->getTriggerTime()) == (rhs->getTriggerTime())) ? (lhs_pri < rhs_pri): ((lhs->getTriggerTime()) < (rhs->getTriggerTime()));
    else 
        return ((lhs->getTriggerTime()) == (rhs->getTriggerTime())) ? (lhs_pri > rhs_pri): ((lhs->getTriggerTime()) > (rhs->getTriggerTime()));
}

class recv_event: public event {
    public:
        class recv_data; // forward declaration
            
    private:
        recv_event(recv_event&) {} // this constructor cannot be used
        recv_event() {} // we don't allow users to new a recv_event by themselv
        unsigned int senderID; // the sender
        unsigned int receiverID; // the receiver 
        packet *pkt; // the packet
        
    protected:
        // this constructor cannot be directly called by users; only by generator
        recv_event(unsigned int _trigger_time, void *data): event(_trigger_time), senderID(BROCAST_ID), receiverID(BROCAST_ID), pkt(nullptr){
            recv_data * data_ptr = (recv_data*) data;
            senderID = data_ptr->s_id;
            receiverID = data_ptr->r_id;
            pkt = data_ptr->_pkt;
        } 
        
    public:
        virtual ~recv_event(){}
        // recv_event will trigger the recv function
        virtual void trigger();
        
        unsigned int event_priority() const;
        
        class recv_event_generator;
        friend class recv_event_generator;
        // recv_event is derived from event_generator to generate a event
        class recv_event_generator : public event_generator{
                static recv_event_generator sample;
                // this constructor is only for sample to register this event type
                recv_event_generator() { /*cout << "recv_event registered" << endl;*/ register_event_type(&sample); }
            protected:
                virtual event * generate(unsigned int _trigger_time, void *data){ 
                    // cout << "recv_event generated" << endl; 
                    return new recv_event(_trigger_time, data); 
                }
                
            public:
                virtual string type() { return "recv_event";}
                ~recv_event_generator(){}
        };
        // this class is used to initialize the recv_event
        class recv_data{
            public:
                unsigned int s_id;
                unsigned int r_id;
                packet *_pkt;
        };
        
        void print () const;
};
recv_event::recv_event_generator recv_event::recv_event_generator::sample;

void recv_event::trigger() {
    if (pkt == nullptr) {
        cerr << "recv_event error: no pkt!" << endl; 
        return ; 
    }
    else if (node::id_to_node(receiverID) == nullptr){
        cerr << "recv_event error: no node " << receiverID << "!" << endl;
        delete pkt; return ;
    }
    node::id_to_node(receiverID)->recv(pkt); 
}
unsigned int recv_event::event_priority() const {
    string string_for_hash;
    string_for_hash = to_string(getTriggerTime()) + to_string(senderID) + to_string (receiverID) + to_string (pkt->getPacketID());
    return get_hash_value(string_for_hash);
}
// the recv_event::print() function is used for log file
void recv_event::print () const {
    cout << "time "          << setw(11) << event::getCurTime() 
         << "   recID "      << setw(11) << receiverID 
         << "   pktID"       << setw(11) << pkt->getPacketID()
         << "   srcID "      << setw(11) << pkt->getHeader()->getSrcID() 
         << "   dstID"       << setw(11) << pkt->getHeader()->getDstID() 
         << "   preID"       << setw(11) << pkt->getHeader()->getPreID()
         << "   nexID"       << setw(11) << pkt->getHeader()->getNexID()
         << endl;
}

class send_event: public event {
    public:
        class send_data; // forward declaration
            
    private:
        send_event (send_event &){}
        send_event (){} // we don't allow users to new a recv_event by themselves
        // this constructor cannot be directly called by users; only by generator
        unsigned int senderID; // the sender
        unsigned int receiverID; // the receiver 
        packet *pkt; // the packet
    
    protected:
        send_event (unsigned int _trigger_time, void *data): event(_trigger_time), senderID(BROCAST_ID), receiverID(BROCAST_ID), pkt(nullptr){
            send_data * data_ptr = (send_data*) data;
            senderID = data_ptr->s_id;
            receiverID = data_ptr->r_id;
            pkt = data_ptr->_pkt;
        } 
        
    public:
        virtual ~send_event(){}
        // send_event will trigger the send function
        virtual void trigger();
        
        unsigned int event_priority() const;
        
        class send_event_generator;
        friend class send_event_generator;
        // send_event is derived from event_generator to generate a event
        class send_event_generator : public event_generator{
                static send_event_generator sample;
                // this constructor is only for sample to register this event type
                send_event_generator() { /*cout << "send_event registered" << endl;*/ register_event_type(&sample); }
            protected:
                virtual event * generate(unsigned int _trigger_time, void *data){ 
                    // cout << "send_event generated" << endl; 
                    return new send_event(_trigger_time, data); 
                }
            
            public:
                virtual string type() { return "send_event";}
                ~send_event_generator(){}
        };
        // this class is used to initialize the send_event
        class send_data{
            public:
                unsigned int s_id;
                unsigned int r_id;
                packet *_pkt;
        };
        
        void print () const;
};
send_event::send_event_generator send_event::send_event_generator::sample;

void send_event::trigger() {
    if (pkt == nullptr) {
        cerr << "send_event error: no pkt!" << endl; 
        return ; 
    }
    else if (node::id_to_node(senderID) == nullptr){
        cerr << "send_event error: no node " << senderID << "!" << endl;
        delete pkt; return ;
    }
    node::id_to_node(senderID)->send(pkt);
}
unsigned int send_event::event_priority() const {
    string string_for_hash;
    string_for_hash = to_string(getTriggerTime()) + to_string(senderID) + to_string (receiverID) + to_string (pkt->getPacketID());
    return get_hash_value(string_for_hash);
}
// the send_event::print() function is used for log file
void send_event::print () const {
    cout << "time "          << setw(11) << event::getCurTime() 
         << "   senID "      << setw(11) << senderID 
         << "   pktID"       << setw(11) << pkt->getPacketID()
         << "   srcID "      << setw(11) << pkt->getHeader()->getSrcID() 
         << "   dstID"       << setw(11) << pkt->getHeader()->getDstID() 
         << "   preID"       << setw(11) << pkt->getHeader()->getPreID()
         << "   nexID"       << setw(11) << pkt->getHeader()->getNexID()
         //<< "   msg"         << setw(11) << dynamic_cast<GR_payload*>(pkt->getPayload())->getMsg()
         << endl;
}


class link {
        // all links created in the program
        static map< pair<unsigned int,unsigned int>, link*> id_id_link_table;
        
        unsigned int id1; // from
        unsigned int id2; // to
        
    protected:
        link(link&){} // this constructor should not be used
        link(){} // this constructor should not be used
        link(unsigned int _id1, unsigned int _id2): id1(_id1), id2(_id2) { id_id_link_table[pair<unsigned int,unsigned int>(id1,id2)] = this; }

    public:
        virtual ~link() { 
            id_id_link_table.erase (pair<unsigned int,unsigned int>(id1,id2)); // erase the link
        }
        
        static link * id_id_to_link (unsigned int _id1, unsigned int _id2) { 
            return ((id_id_link_table.find(pair<unsigned int,unsigned int>(_id1,_id2))!=id_id_link_table.end()) ? id_id_link_table[pair<unsigned,unsigned>(_id1,_id2)]: nullptr) ; 
        }

        virtual double getLatency() = 0; // you must implement your own latency
        
        static void del_link (unsigned int _id1, unsigned int _id2) {
            pair<unsigned int, unsigned int> temp;
            if (id_id_link_table.find(temp)!=id_id_link_table.end())
                id_id_link_table.erase(temp); 
        }

        static unsigned int getLinkNum () { return id_id_link_table.size(); }

        class link_generator {
                // lock the copy constructor
                link_generator(link_generator &){}
                // store all possible types of link
                static map<string,link_generator*> prototypes;
            protected:
                // allow derived class to use it
                link_generator() {}
                // after you create a new link type, please register the factory of this link type by this function
                void register_link_type(link_generator *h) { prototypes[h->type()] = h; }
                // you have to implement your own generate() to generate your link
                virtual link* generate(unsigned int _id1, unsigned int _id2) = 0;
            public:
                // you have to implement your own type() to return your link type
        	    virtual string type() = 0;
        	    // this function is used to generate any type of link derived
        	    static link * generate (string type, unsigned int _id1, unsigned int _id2) {
        	        if(id_id_link_table.find(pair<unsigned int,unsigned int>(_id1,_id2))!=id_id_link_table.end()){
        	            std::cerr << "duplicate link id" << std::endl; // link id is duplicated
        	            return nullptr;
        	        }
        	        else if ( BROCAST_ID == _id1 || BROCAST_ID == _id2 ) {
        	            cerr << "BROCAST_ID cannot be used" << endl;
        	            return nullptr;
        	        }
            		else if (prototypes.find(type) != prototypes.end()){ // if this type derived exists 
            		    link * created_link = prototypes[type]->generate(_id1,_id2);
            			return created_link; // generate it!!
            		}
            		std::cerr << "no such link type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static void print () {
            	    cout << "registered link types: " << endl;
            	    for (map<string,link::link_generator*>::iterator it = prototypes.begin(); it != prototypes.end(); it ++)
            	        cout << it->second->type() << endl;
            	}
            	virtual ~link_generator(){};
        };
};
map<string,link::link_generator*> link::link_generator::prototypes;
map<pair<unsigned int,unsigned int>, link*> link::id_id_link_table;

void node::add_phy_neighbor (unsigned int _id, string link_type){
    if (id == _id) return; // if the two nodes are the same...
    if (id_node_table.find(_id)==id_node_table.end()) return; // if this node does not exist
    if (phy_neighbors.find(_id)!=phy_neighbors.end()) return; // if this neighbor has been added
    phy_neighbors[_id] = true;
    
    link::link_generator::generate(link_type,id,_id);
}
void node::del_phy_neighbor (unsigned int _id){
    phy_neighbors.erase(_id);
    
}


class simple_link: public link {
    protected:
        simple_link() {} // it should not be used outside the class
        simple_link(simple_link&) {} // it should not be used
        simple_link(unsigned int _id1, unsigned int _id2): link (_id1,_id2){} // this constructor cannot be directly called by users
    
    public:
        virtual ~simple_link() {}
        virtual double getLatency() { return ONE_HOP_DELAY; } // you must implement your own latency
        
        class simple_link_generator;
        friend class simple_link_generator;
        // simple_link is derived from link_generator to generate a link
        class simple_link_generator : public link_generator {
                static simple_link_generator sample;
                // this constructor is only for sample to register this link type
                simple_link_generator() { /*cout << "simple_link registered" << endl;*/ register_link_type(&sample); }
            protected:
                virtual link * generate(unsigned int _id1, unsigned int _id2) 
                { /*cout << "simple_link generated" << endl;*/ return new simple_link(_id1,_id2); }
            public:
                virtual string type() { return "simple_link"; }
                ~simple_link_generator(){}
        };
};

simple_link::simple_link_generator simple_link::simple_link_generator::sample;

class GR_node: public node {
        double x;
        double y;
        // map<unsigned int,bool> one_hop_neighbors; // you can use this variable to record the node's 1-hop neighbors 
        
        bool hi; // this is used for example
		map<unsigned int  , bool> nblist; // æF©~¶C™Ì 
		map<unsigned int, pair<double,double>> node_co; // •Œ®”¶s®C≠”deadend™∫Æyº– 
		//ccu
		queue<packet*> cache;
    protected:
        GR_node() {} // it should not be used
        GR_node(GR_node&) {} // it should not be used
        GR_node(unsigned int _id): node(_id), hi(false) {} // this constructor cannot be directly called by users
    
    public:
        ~GR_node(){}
        
        SET(setX,double,x,_x);
        SET(setY,double,y,_y);
        GET(getX,double,x);
        GET(getY,double,y);
        
        // please define recv_handler function to deal with the incoming packet
        virtual void recv_handler (packet *p);
        
        // void add_one_hop_neighbor (unsigned int n_id) { one_hop_neighbors[n_id] = true; }
        // unsigned int get_one_hop_neighbor_num () { return one_hop_neighbors.size(); }
        
        class GR_node_generator;
        friend class GR_node_generator;
        // GR_node is derived from node_generator to generate a node
        class GR_node_generator : public node_generator{
                static GR_node_generator sample;
                // this constructor is only for sample to register this node type
                GR_node_generator() { /*cout << "GR_node registered" << endl;*/ register_node_type(&sample); }
            protected:
                virtual node * generate(unsigned int _id){ /*cout << "GR_node generated" << endl;*/ return new GR_node(_id); }
            public:
                virtual string type() { return "GR_node";}
                ~GR_node_generator(){}
        };
};
GR_node::GR_node_generator GR_node::GR_node_generator::sample;


// the function is used to add an initial event
void add_initial_event (unsigned int src, unsigned int dst, unsigned t = 0, string msg = "default"){
    if ( node::id_to_node(src) == nullptr || (dst != BROCAST_ID && node::id_to_node(dst) == nullptr) ) {
        cerr << "src or dst is incorrect" << endl; return ;
        return;
    }
    GR_packet *pkt = dynamic_cast<GR_packet*> ( packet::packet_generator::generate("GR_packet") );
    if (pkt == nullptr) { 
        cerr << "packet type is incorrect" << endl; return; 
    }
    GR_header *hdr = dynamic_cast<GR_header*> ( pkt->getHeader() );
    GR_payload *pld = dynamic_cast<GR_payload*> ( pkt->getPayload() );
    
    if (hdr == nullptr) {
        cerr << "header type is incorrect" << endl; return ;
    }
    if (pld == nullptr) {
        cerr << "payload type is incorrect" << endl; return ;
    }
    
    GR_node *dst_n = nullptr;
    
    if ( dst != BROCAST_ID) {
        dst_n = dynamic_cast<GR_node*> (node::id_to_node(dst));
        if ( dst_n == nullptr) {
            cerr << "node type is incorrect" << endl; return ;
        }
        hdr->setDstX( dst_n->getX() );
        hdr->setDstY( dst_n->getY() );
    }

    hdr->setSrcID(src);
    hdr->setDstID(dst);
    hdr->setPreID(src);
    hdr->setNexID(src);
    hdr->setDstInfo(-666);
    
	pld->setMsg(msg);
    
    recv_event::recv_data e_data;
    e_data.s_id = src;
    e_data.r_id = src;
    e_data._pkt = pkt;
    
    // for (unsigned int i = 0; i < 10; i ++){
    //     hdr->push_visited_node(i);
    // }
    // for (unsigned int i = 0; i < 5; i ++){
    //     hdr->mark_visited_node(i);
    // }
    
    recv_event *e = dynamic_cast<recv_event*> ( event::event_generator::generate("recv_event",t, (void *)&e_data) );
    if (e == nullptr) cerr << "event type is incorrect" << endl;
}
//ccu
void add_initial_event2 (unsigned int src, unsigned int dst, unsigned t = 0, string msg = "default"){ // t¨Obroadcast time

    if ( node::id_to_node(src) == nullptr || (dst != BROCAST_ID && node::id_to_node(dst) == nullptr) ) {
        cerr << "src or dst is incorrect" << endl; return ;
        return;
    }
    Hello_packet *pkt = dynamic_cast<Hello_packet*> ( packet::packet_generator::generate("Hello_packet") );

    if (pkt == nullptr  ) { 
        cerr << "packet type is incorrect2" << endl; return; 
    }
    
    Hello_header *hdr = dynamic_cast<Hello_header*> ( pkt->getHeader() );
    Hello_payload *pld = dynamic_cast<Hello_payload*> ( pkt->getPayload() );
    
    if (hdr == nullptr) {
        cerr << "header type is incorrect" << endl; return ;
    }
    if (pld == nullptr ) {
        cerr << "payload type is incorrect" << endl; return ;
    }
    
    GR_node *dst_n = nullptr;

    if ( dst != BROCAST_ID) { // 
        dst_n = dynamic_cast<GR_node*> (node::id_to_node(dst));
        if ( dst_n == nullptr) {
            cerr << "node type is incorrect" << endl; return ;
        }
        hdr->setDstX( dst_n->getX() );
        hdr->setDstY( dst_n->getY() );
    }

    hdr->setSrcID(src);
    hdr->setDstID(dst);
    hdr->setPreID(src);
    hdr->setNexID(BROCAST_ID);

    pld->setMsg(msg);
    
    recv_event::recv_data e_data;
    e_data.s_id = src;
    e_data.r_id = src;
    e_data._pkt = pkt;
  
    recv_event *e = dynamic_cast<recv_event*> ( event::event_generator::generate("recv_event",t, (void *)&e_data) );
    if (e == nullptr) cerr << "event type is incorrect" << endl;
} // add_initial_event2() 


void setNodePos (unsigned int id, pair<double, double> pos){
    if ( id == BROCAST_ID ) {
        cerr << "node id is incorrect" << endl; return ;
    }
    GR_node* ptr = dynamic_cast <GR_node*> (node::id_to_node(id));
    if ( ptr == nullptr) {
            cerr << "node type is incorrect" << endl; return ;
    }
    ptr->setX(pos.first);
    ptr->setY(pos.second);
}
pair<double, double> getNodePos (unsigned int id){
    if ( id == BROCAST_ID ) {
        cerr << "node id is incorrect" << endl; return pair<double,double> (0,0);
    }
    GR_node* ptr = dynamic_cast <GR_node*> (node::id_to_node(id));
    if ( ptr == nullptr) {
            cerr << "node type is incorrect" << endl; return pair<double,double> (0,0);
    }
    return pair<double, double> (ptr->getX(),ptr->getY());
}

// send_handler function is used to transmit packet p based on the information in the header
// Note that the packet p will not be discard after send_handler ()

void node::send_handler(packet *p){
    packet *_p = packet::packet_generator::replicate(p);
    send_event::send_data e_data;
    e_data.s_id = _p->getHeader()->getPreID();
    e_data.r_id = _p->getHeader()->getNexID();
    e_data._pkt = _p;
    send_event *e = dynamic_cast<send_event*> (event::event_generator::generate("send_event",event::getCurTime(), (void *)&e_data) );
    if (e == nullptr) cerr << "event type is incorrect" << endl;
}

void node::send (packet *p){ // this function is called by event; not for the user
    if (p == nullptr) return;
    
    unsigned int _nexID = p->getHeader()->getNexID();
    for ( map<unsigned int,bool>::iterator it = phy_neighbors.begin(); it != phy_neighbors.end(); it ++) {
        unsigned int nb_id = it->first; // neighbor id
        
        if (nb_id != _nexID && BROCAST_ID != _nexID) continue; // this neighbor will not receive the packet
        
        unsigned int trigger_time = event::getCurTime() + link::id_id_to_link(id, nb_id)->getLatency() ; // we simply assume that the delay is fixed
        // cout << "node " << id << " send to node " <<  nb_id << endl;
        recv_event::recv_data e_data;
        e_data.s_id = id;
        e_data.r_id = nb_id;
        
        packet *p2 = packet::packet_generator::replicate(p);
        e_data._pkt = p2;
        
        recv_event *e = dynamic_cast<recv_event*> (event::event_generator::generate("recv_event", trigger_time, (void*) &e_data)); // send the packet to the neighbor
        if (e == nullptr) cerr << "event type is incorrect" << endl;
    }
    packet::discard(p);
}

double dis(double x1, double y1, double x2, double y2){
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}


// you have to write the code in recv_handler
void GR_node::recv_handler (packet *p){

    // this is a simple example
    // node 0 broadcasts its message to every node and every node relays the packet "only once"
    // the variable hi is used to examine whether the packet has been received before
    // you can remove the variable hi and create your own variables in class GR_node
    GR_packet * p2 = nullptr;
	Rep_packet * p3 = nullptr;
	Ret_packet * p4 = nullptr;
	Res_packet * p5 = nullptr;
	Hello_packet * p6 = nullptr;

    if (p->type() == "GR_packet" && !hi ){ // greedy routing
    // •˝ΩTª{ DesInfo¶p™G®S¶≥∏ÍÆ∆°A¨›æF©~∏ÍÆ∆°A¶≥->®˙∏ÍÆ∆®√∂«∞e°A
	// µL->•hß‰Deadend≠n∏ÍÆ∆ °A¶≥->∞eRes°AµL->§£≠n∞ ß@ 
    // ≠Y¶¨®ÏRes±qqueueÆ≥•Xº»¶s°A®√∞µßπ°C 
		//cout << p->type() <<endl;

		p2 = dynamic_cast<GR_packet*> (p); // GR_packet 
        int target_id; // send to target_id
        int des_id = p2 -> getHeader() -> getDstID(); //
        int now_id;
        double x,y,x2,y2,x1,y1;
        double min_dis;
        double origin;
        double deadend_x,deadend_y;
        bool Is_neighbor = false;
        
        x2 = (dynamic_cast<GR_node*>(node::id_to_node(des_id)))->getX();
        y2 = (dynamic_cast<GR_node*>(node::id_to_node(des_id)))->getY();
        now_id = getNodeID();
        // x,y is now x,y
        x = (dynamic_cast<GR_node*>(node::id_to_node(now_id)))->getX();
        y = (dynamic_cast<GR_node*>(node::id_to_node(now_id)))->getY();
        //minimum distance is min_dis between now and destination
        min_dis = dis(x,y,x2,y2);
        origin = min_dis;
        
        
        // cout << "somethiong " << p2->getHeader()->getDstInfo() <<endl;
        for(map<unsigned int,bool>::const_iterator it = nblist.begin(); it != nblist.end(); it ++) { // dst¨OæF©~ 
			if( it->first == des_id ){
				Is_neighbor = true;
				//cout << "RE?"<<endl;
            	p2->getHeader()->setDstInfo(des_id);
            } // if()
        } // for()
        for(map<unsigned int,pair<double,double>>::const_iterator it = node_co.begin(); it != node_co.end(); it ++) { // ©Œ§w∏g¶s¶b¶€§v•ª®≠         
			//cout << it->first << "   BOOOOOOMMMMMM!!!!!" <<endl;
			 if( it->first == des_id ) {

				Is_neighbor = true;
				//cout << "RE?"<<endl;
            	p2->getHeader()->setDstInfo(des_id);
            } // if()
        } // for
        
        
        //cout << Is_neighbor << endl; 
        //cout << "getDstInfo() " <<p2->getHeader()->getDstInfo() <<" des_id : " <<  des_id <<endl; 
        
		if( p2->getHeader()->getDstInfo() != des_id ) { //dst§£¶bnb§§°Aª›≠nº»¶sGR_packet°A®√•Õ¶®§@≠”Ret_packet 
			// p4 for Res_packet
			// cout << "here  "<<endl;	
			packet *p15 = packet::packet_generator::replicate(p); 
			cache.push(p15);
			
			packet *p4 = dynamic_cast<Ret_packet*> ( packet::packet_generator::generate("Ret_packet") );
			packet *hdr = dynamic_cast<Ret_packet*> (p4->getHeader() );
			packet *pld = dynamic_cast<Ret_packet*> (p4->getPayload() );

			// p4->getHeader()->setSrcID( p2->getHeader()->getSrcID());
			p4->getHeader()->setSrcID( getNodeID());
			p4->getHeader()->setDstInfo(des_id);//≠n∞›•ÿ™∫¶a™∫data 
			p4->getHeader()->setDstID(BROCAST_ID);
			
			Hash_function(des_id,deadend_x,deadend_y,Xmax,Ymax);
	        now_id = getNodeID();
	        // x,y is now x,y
	        x = (dynamic_cast<GR_node*>(node::id_to_node(now_id)))->getX();
	        y = (dynamic_cast<GR_node*>(node::id_to_node(now_id)))->getY();
	        //minimum distance is min_dis between now and destination
	        min_dis = dis(x,y,deadend_x,deadend_y);
	        origin = min_dis;
			
            cout << nblist.size() << endl;
            cout << "#####     " <<getNodeID() << endl;
            cout << "dead end ==   " <<  deadend_x <<"    "<< deadend_y <<  endl;
			for(map<unsigned int,bool>::const_iterator it = nblist.begin(); it != nblist.end(); it ++) {
            //ß‰¬˜Deadend≥Ã™Ò™∫æF©~ 
                cout << "!!!!" << it->first << endl;
	            x1 = (dynamic_cast<GR_node*>(node::id_to_node(it -> first)))->getX();
	            y1 = (dynamic_cast<GR_node*>(node::id_to_node(it -> first)))->getY();
                cout <<"DIS = " << dis(x1,y1,deadend_x,deadend_y) << endl;
                cout << "COOR"  << getNodePos(it->first).first << "   "<< getNodePos(it->first).second << endl;
	            if(dis(x1,y1,deadend_x,deadend_y) < min_dis){ // if the distance is closer?
	                min_dis = dis(x1,y1,deadend_x,deadend_y);
	                target_id = it -> first; // change the closer id
            	} // if()
        	}//for()
	        if(origin == min_dis){//•N™Ì®S§SßÛ™Ò™∫
	            return;
	        }//if()
            cout << "@@@@@" << target_id << endl;
	        p4->getHeader()->setPreID ( getNodeID() );//pre -> now id
	        p4->getHeader()->setNexID ( target_id );//nex -> target_id
	        //cout << "sned Ret_packet ......" <<endl;
			send_handler(p4);
			packet::discard(p4);
			return;
		} //if()
		if(des_id == getNodeID()){ //if is final node, stop.
            return; //¿≥∏”¶b¶€§v•ª®≠´o®S¶≥°A•iØ‡¨O®S¶≥LINK ©Œ¡Ÿ®S¶≥∞e®Ï 
        } // if()
        // Greedy for GR_packet 
        for(map<unsigned int,bool>::const_iterator it = nblist.begin(); it != nblist.end(); it ++) {
            //cout << "!!!"<<it->first << endl;
            x1 = (dynamic_cast<GR_node*>(node::id_to_node(it -> first)))->getX();
            y1 = (dynamic_cast<GR_node*>(node::id_to_node(it -> first)))->getY();
            if(dis(x1,y1,x2,y2) < min_dis){ // if the distance is closer?
                min_dis = dis(x1,y1,x2,y2);
                target_id = it -> first; // change the closer id
            } // if()
        }//for()
        if(origin == min_dis){//if origin == min_dis, means it doesn't change. stop it.
            return;
        }//if()
        //cout <<"!!!!!!!!!!!!!!!"<<endl;
        p2->getHeader()->setPreID ( getNodeID() );//pre -> now id
        p2->getHeader()->setNexID ( target_id );//nex -> target_id
        send_handler (p2);//send it
    
    } // if(greedy routing)
    else if(p->type() == "Hello_packet" && !hi) {
    	//cout << p->type() <<endl;
    	p6 = dynamic_cast<Hello_packet*> (p);
    	if( p6->getHeader()->getSrcID() == getNodeID() ) send_handler(p6);
    	else {
    		nblist[p6->getHeader()->getSrcID()] = true;
		}
	} // else if(Hello)


    // you should implement the GR distributed algorithm in recv_hander
    // getNodeID() returns the id of the current node
    
    // The current node's neighbors are already stored in the following variable 
    // map<unsigned int,bool> node::phy_neighbors
    // however, this variable is private in the class node
    // You have to use node::getPhyNeighbors to get the variable
    // for example, if you want to print all the neighbors of this node
    // const map<unsigned int,bool> &nblist = getPhyNeighbors();
    // for (map<unsigned int,bool>::const_iterator it = nblist.begin(); it != nblist.end(); it ++) {
    //     cout << it->first << endl;
    // }

    // you can use p->getHeader()->setSrcID() or getSrcID()
    //             p->getHeader()->setDstID() or getDstID()
    //             p->getHeader()->setPreID() or getPreID()
    //             p->getHeader()->setNexID() or getNexID() to change or read the packet header
    
    // In addition, you can get the packet, header, and payload with the correct type (GR)
    // in fact, this is downcasting
    // GR_packet * pkt = dynamic_cast<GR_packet*> (p);
    // GR_header * hdr = dynamic_cast<GR_header*> (p->getHeader());
    // GR_payload * pld = dynamic_cast<GR_payload*> (p->getPayload());
    
    // you can also change the GR_header setting
    // hdr->setDstX(double): to set the destination's position
    // hdr->setDstY(double)
    
    // you can also change the GR_payload setting
    // pld->setMsg(string): to set the message transmitted to the destination
    
    // Besides, you can use packet::packet_generator::generate() to generate a new packet; note that you should fill the header and payload in the packet
    // moreover, you can use "packet *p2 = packet::packet_generator::replicate(p)" to make a clone p2 of packet p
    // note that if the packet is generated or replicated manually, you must delete it by packet::discard() manually before recv_hander finishes
    
    // "IMPORTANT":
    // You have to "carefully" fill the correct information (e.g., srcID, dstID, ...) in the packet before you send it
    // Note that if you want to transmit a packet to only one next node (i.e., unicast), then you fill the ID of a specific node to "nexID" in the header
    // Otherwise, i.e., you want to broadcasts, then you fill "BROCAST_ID" to "nexID" in the header
    // after that, you can use send() to transmit the packet 
    // usage: send_handler (p);
    
    // note that packet p will be discarded (deleted) after recv_hander(); you don't need to manually delete it
}

bool is_neighbor(double x1,double y1,double x2,double y2){
    double dis = ( x1 - x2 ) * ( x1 - x2 ) + ( y1 - y2 ) * ( y1 - y2 );
    return dis > 1 ? 0 : 1;
}

int main() {

    int case_; // node º∆∂q 
    cin >> case_;
    unsigned int xmax,ymax;
	cin >> xmax >> ymax;
	Xmax = xmax;
	Ymax = ymax; 
    int n = case_;
    // input
    for (unsigned int id = 0; id < case_; id ++) {
        node::node_generator::generate("GR_node",id);
    }
    // make node
    for(int i = 0; i < case_; case_--){
        int id,broadcast_time,Rep_time;
        double x, y;
        cin >> id >> x >> y >> broadcast_time >> Rep_time;
        (dynamic_cast<GR_node*>(node::id_to_node(id)))->setX(x);
        (dynamic_cast<GR_node*>(node::id_to_node(id)))->setY(y);
        add_initial_event2(id, BROCAST_ID, broadcast_time);
		add_initial_event3(id, BROCAST_ID, Rep_time);
    }
    // Is neighbor?
    for(int i = 0 ; i < n ; i++) { 
	        double x1 = (dynamic_cast<GR_node*>(node::id_to_node(i)))->getX();
	        double y1 = (dynamic_cast<GR_node*>(node::id_to_node(i)))->getY();
	        for(int j = i + 1; j < n ; j++) {
	            double x2 = (dynamic_cast<GR_node*>(node::id_to_node(j)))->getX();
	            double y2 = (dynamic_cast<GR_node*>(node::id_to_node(j)))->getY();
	            double dis = ( x1 - x2 ) * ( x1 - x2 ) + ( y1 - y2 ) * ( y1 - y2 );
	            if(dis <= 1 ){ // is neighber? if yes, add it.
	                node::id_to_node(i)->add_phy_neighbor(j);
	                node::id_to_node(j)->add_phy_neighbor(i);
	            } // if()
	        } // for()
    	} // for() 
    
    //input
    cin >> case_;
    int times;
    cin >> times;
    
    while(case_--){
        int src, dst, t; //initial packet
        cin >> t >> src >> dst;
        // cout << t <<" "<< src <<" "<< dst;
        add_initial_event(src, dst, t);
    } // while()
    event::start_simulate(times);
    return 0;
}


