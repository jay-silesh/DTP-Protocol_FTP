#include "../netsim/common.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/Packet.h"
#include "CbrPacket.h"
#include "../netsim/PacketScheduler.h"
#include "../netsim/Timer.h"
#include "../netsim/Scheduler.h"
#include "CbrReceiver.h"

CbrReceiver::CbrReceiver(Address a) : FIFONode(a, 
                                               0)	// Null queue size
{
    // Empty
}

CbrReceiver::~CbrReceiver()
{
    // Empty
}

void
CbrReceiver::receive(Packet* pkt)
{
    ((CbrPacket*) pkt)->print();
    delete pkt;
}
