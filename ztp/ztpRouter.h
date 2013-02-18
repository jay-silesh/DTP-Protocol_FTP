class FIFONode;

class ztpRouter : public FIFONode {
 public:
     ztpRouter(Address a,int q_size);		// Creating a Router...
    ~ztpRouter();

    // Handle a timer
  //  void handle_timer(void* cookie);
    
   /* Address	destination;	// Target address
    Time	start;			// Start sending at
    Time	inter_packet_time;	// Inter-packet time
    int		packets_to_send;	// number of packets
    int		sent_so_far;		// number sent
    char * file_holder;			//contains the file location from the sender's side..
    */

};
