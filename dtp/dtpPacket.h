#include <cstdio>

#define PAYLOAD_SIZE (MTU - sizeof(Packet))

//
// CbrPacket is derived from Packet and adds a data field.
//

class dtpPacket : public Packet {
 public:
    void print();
    bool syn;
    bool ack;
    bool fin;
    char *data;
    bool last_packet;
    //Time time_out;
  	dtpPacket ();
    dtpPacket ( dtpPacket& pck );
 };