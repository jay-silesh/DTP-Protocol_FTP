#include "../netsim/common.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/Packet.h"
#include "ztpPacket.h"
#include "../netsim/Timer.h"
#include "../netsim/PacketScheduler.h"
#include "../netsim/Scheduler.h"

void
ztpPacket::print()
{
    Packet::print_header();
   // Packet::print_payload((char *) &data[0], length - sizeof(Packet), false);
    TRACE(TRL3, "SYN %d, ACK %d, FIN: %d,SRC: %d \n",
          this->syn,this->ack,this->fin,source);

}

