class FIFONode;

class ztpReceiver : public FIFONode {
 public:
    ztpReceiver(Address a);
    ~ztpReceiver();

    // Receive packet and prints its length
    void receive(Packet* pkt);		// Incoming packet
};
