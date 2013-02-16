class FIFONode;

class ztpSender : public FIFONode {
 public:
    ztpSender(Address a,		// Unique node address
              Address d,		// Target
              Time s,			// Start time of node
              void* file_name);			// Number of packets to send
    ~ztpSender();

    // Handle a timer
    void handle_timer(void* cookie);

 private:
    Address	destination;		// Target address
    Time	start;			// Start sending at
    Time	inter_packet_time;	// Inter-packet time
    int		packets_to_send;	// number of packets
    int		sent_so_far;		// number sent
};
