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
    char *data;
    //Time time_out;
  	ztpPacket ();
    ztpPacket ( ztpPacket& pck );
 };