class FIFONode;

class ztpHost : public FIFONode {
 public:
    ztpHost(Address a);
    ~ztpHost();

    // Receive packet and prints its length
    void receive(Packet* pkt);		// Incoming packet
    
};
