#include <cstdio>

#define PAYLOAD_SIZE (MTU - sizeof(dtpPacket))

//
// CbrPacket is derived from Packet and adds a data field.
//

class dtpPacket : public Packet {
 public:
    void print_receiver();
    void print_receiver_app();
    void print_sender();
    void print_resender();
    void print_payload2(char *p,unsigned int len,bool ascii);
    bool syn;
    bool ack;
    bool fin;
    char *data;
    dtpPacket ();
    dtpPacket ( dtpPacket& pck );
    dtpPacket ( Address source_1, Address destination_1,unsigned int length_1);

    /* Used for cummulative ,window,etc ...MAKE CHANGES IN DTPPACKET.h*/
    bool last_packet;
    unsigned short cwnd_calculated;

};
