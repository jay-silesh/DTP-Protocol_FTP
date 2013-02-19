class FIFONode;

class ztpHost : public FIFONode {
 public:
     ztpHost(Address a);		// Creating a Router...
    ~ztpHost();

    // Handle a timer
    void handle_timer(void* cookie);
    void receive(Packet* pkt);	
    void FDTP(Address s,Address d,Time start_time,char *p);

    Address	destination;	// Target address
    Time	start;			// Start sending at
    Time	inter_packet_time;	// Inter-packet time
    
    int		packets_to_send;	// number of packets
    int		sent_so_far;		// number sent
    char    *file_holder;			//contains the file location from the sender's side..
    bool    syn_recieved;
    bool    normal_packet;
    bool    finish_packet;
};
