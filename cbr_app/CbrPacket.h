#include <cstdio>

#define PAYLOAD_SIZE (MTU - sizeof(Packet))

//
// CbrPacket is derived from Packet and adds a data field.
//

class CbrPacket : public Packet {
 public:
    void print();

    unsigned char data[PAYLOAD_SIZE];
};
