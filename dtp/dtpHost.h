#include "new_library.h"

class FIFONode;


typedef map<unsigned int, Packet*, ltid> RetransmissionPacketMap;
typedef map<unsigned int, Packet*, ltid>::iterator RetransmissionPacketMapIterator;
typedef pair<unsigned int,Packet*> RetransmissionPacketMapPair;


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
    unsigned int packet_expected_sender_side;
    unsigned int normal_packet_received;  //At the receiver side , to know which NORMAL packets has been recieved


    enum
    {
       SYN,SYN_ACK,FIN,FIN_ACK,listening,sending,FIN_DONE,START_SENDING_FIN 
    } state;
    bool sender;
    void set_packet( Packet* pkt_p,bool syn_p,bool ack_p,bool fin_p);
    void set_packet( Packet* pkt_p,bool syn_p,bool ack_p,bool fin_p,unsigned int id_p);
    void delete_retransmission_timmer(int packet_no);
    void set_retransmission();
    void set_retransmission_map(Packet *pkt_t);
    void set_retransmission_cookie(unsigned int , int);
    void set_normal_cookie();
    void check_inorder_packets();
  
    RetransmissionPacketMap  re_packet_map;
    InorderPacketMap order_packet_map;

};

