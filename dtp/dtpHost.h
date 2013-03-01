#include "new_library.h"

class FIFONode;


typedef map<unsigned int, Packet*, ltid> RetransmissionPacketMap;
typedef map<unsigned int, Packet*, ltid>::iterator RetransmissionPacketMapIterator;
typedef pair<unsigned int,Packet*> RetransmissionPacketMapPair;





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
    int		packets_to_send;	// number of packets
    int		sent_so_far;		// number sent
    char * file_holder;
    
    unsigned int packets_rec;
    
    enum
    {
       SYN,SYN_ACK,FIN,FIN_ACK,listening,sending,FIN_DONE,START_SENDING_FIN 
    } state;
    bool sender;
    void set_packet(Packet* pkt_p,bool syn_p,bool ack_p,bool fin_p);
    void set_packet(Packet* pkt_p,bool syn_p,bool ack_p,bool fin_p,unsigned int id_p);
    RetransmissionPacketMap  re_packet_map;
    void delete_retransmission_timmer(int packet_no);
    void set_retransmission();
    void set_retransmission_map(Packet *pkt_t);
    void set_retransmission_cookie(unsigned int , int);


};

