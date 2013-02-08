#include <cstdio>

#define PAYLOAD_SIZE (MTU - sizeof(Packet))

//
// CbrPacket is derived from Packet and adds a data field.
//

class ztpPacket : public Packet {
 public:
    void print();
    bool syn;
    bool ack;
    bool fin;
    int ack_number;
    unsigned char data[PAYLOAD_SIZE];
    bool check_syn(ztpPacket *);
    bool check_fin(ztpPacket *);
    bool check_asyn(ztpPacket *);
    bool check_packet(ztpPacket *);
};
