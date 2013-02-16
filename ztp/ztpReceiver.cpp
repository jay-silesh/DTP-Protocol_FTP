#include "../netsim/common.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/Packet.h"
#include "ztpPacket.h"
#include "../netsim/PacketScheduler.h"
#include "../netsim/Timer.h"
#include "../netsim/Scheduler.h"
#include "ztpReceiver.h"

ztpReceiver::ztpReceiver(Address a) : FIFONode(a, 
                                               0)	// Null queue size
{
    // Empty
}

ztpReceiver::~ztpReceiver()
{
    // Empty
}

void
ztpReceiver::receive(Packet* pkt)
{
    ((ztpPacket*) pkt)->print();
    delete pkt;
}
