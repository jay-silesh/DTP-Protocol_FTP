#include "new_library.h"
//#include "ztpPacket.h"
class FIFONode;


typedef map<unsigned int, Packet*, ltid> RetransmissionPacketMap;
typedef map<unsigned int, Packet*, ltid>::iterator RetransmissionPacketMapIterator;
typedef pair<unsigned int,Packet*> RetransmissionPacketMapPair;



class ztpHost : public FIFONode {
 public:
     ztpHost(Address a);		// Creating a Router...
    ~ztpHost();

    // Handle a timer
    void handle_timer(void* cookie);
    void receive(Packet* pkt);	
    void FDTP(Address s,Address d,Time start_time,char *p);

    void delete_retransmission_timmer(int packet_no);

    Address	destination;	// Target address
    Time	start;			// Start sending at
    Time	inter_packet_time;	// Inter-packet time
    
    bool    done_transmission;
    int		packets_to_send;	// number of packets
    int		sent_so_far;		// number sent
    bool    syn_recieved;
    bool    normal_packet;
    bool    finish_packet;
    char * file_holder;
    int     cookie_count;

    
    RetransmissionPacketMap  re_packet_map;
    
};

