#include "../netsim/common.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/Packet.h"
#include "CbrPacket.h"
#include "../netsim/Timer.h"
#include "../netsim/PacketScheduler.h"
#include "../netsim/Scheduler.h"

void
CbrPacket::print()
{
    Packet::print_header();
    Packet::print_payload((char *) &data[0], length - sizeof(Packet), false);
}
