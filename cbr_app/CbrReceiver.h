class FIFONode;

class CbrReceiver : public FIFONode {
 public:
    CbrReceiver(Address a);
    ~CbrReceiver();

    // Receive packet and prints its length
    void receive(Packet* pkt);		// Incoming packet
};
