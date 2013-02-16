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
    
    bool check_syn();
    bool check_fin();
    bool check_ack();
    bool check_packet();
};


bool ztpPacket::check_syn()
{
    if(syn==true && ack==false)
        return true;
    return false;
}
bool ztpPacket::check_ack()
{
    if(ack==true && syn==false)
        return true;
    return false;
}

bool ztpPacket::check_packet()
{
    if(syn==false && ack==false && fin==false)
        return true;
    return false;
}

bool ztpPacket::check_fin()
{
    if(fin==true)
        return true;
    return false;
}
