#include "new_library.h"
//#include "../netsim/FIFONode.h"
class FIFONode;
class cookie_class;

typedef map<unsigned int, Packet*, ltid> RetransmissionPacketMap;
typedef map<unsigned int, Packet*, ltid>::iterator RetransmissionPacketMapIterator;
typedef pair<unsigned int,Packet*> RetransmissionPacketMapPair;


typedef map<unsigned int,  cookie_class*, ltid> cookie_timmer_map;
typedef map<unsigned int,  cookie_class*, ltid>::iterator cookie_timmer_map_Iterator;
typedef pair<unsigned int, cookie_class*> cookie_timmer_map_Pair;


typedef map<unsigned int, Packet*, ltid> InorderPacketMap;
typedef map<unsigned int, Packet*, ltid>::iterator InorderPacketMapIterator;
typedef pair<unsigned int,Packet*> InorderPacketMapPair;



class dtpHost : public FIFONode {
 public:
     dtpHost(Address a);		// Creating a Router...
    ~dtpHost();

    // Handle a timer
    void handle_timer(void* cookie);
    void receive(Packet* pkt);	
    void FDTP(Address s,Address d,Time start_time,char *p);

    Address	destination;	// Target address
    bool    done_transmission;
    int		sent_so_far;		// number sent
    char * file_holder;
    
    int rtt_in_host; // The RTT recorded in the host...
    unsigned int packets_rec;
    unsigned int packet_expected;
    unsigned int normal_packet_received;  //At the receiver side , to know which NORMAL packets has been recieved
    unsigned int actual_packets_rec;
    unsigned int lastest_ack_rec;
    int cwnd_host,temp_cwnd_host;  //Congestion window in the host..
    Time rtt_estimation;

    enum
    {
       SYN,SYN_ACK,FIN,FIN_ACK,listening,sending,FIN_DONE,START_SENDING_FIN,sending_ack_listening 
    } state;
    bool sender;
    void set_packet( Packet* pkt_p,bool syn_p,bool ack_p,bool fin_p);
    void set_packet( Packet* pkt_p,bool syn_p,bool ack_p,bool fin_p,unsigned int id_p);
    void delete_retransmission_timmer(int packet_no);
    void set_retransmission();
    void set_retransmission_map(const Packet *pkt_t);
    void set_retransmission_cookie(unsigned int , int);
    void set_normal_cookie();
    bool check_inorder_packets();
    void send_immediately(bool syn_temp,bool ack_temp,bool fin_temp,unsigned int id_temp,Time time_temp,bool);
    int  check_congestion(Packet* pkt_p);
    void delete_timer_cookie(unsigned int pack_no);


    RetransmissionPacketMap  re_packet_map;
    InorderPacketMap order_packet_map;
    queue<Time> queue_for_rtt;
    bool not_first_time_rtt;


    cookie_timmer_map cookie_retranmission_timmer;


};

