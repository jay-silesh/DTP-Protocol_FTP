class FIFONode;

class ztpRouter : public FIFONode {
 public:
    ztpRouter(Address a,int q_size);
    ~ztpRouter();

    // Receive packet and prints its length
    void receive(Packet* pkt);		// Incoming packet
};
