#include "../netsim/common.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/Packet.h"
#include "ztpPacket.h"
#include "../netsim/Timer.h"
#include "../netsim/PacketScheduler.h"
#include "../netsim/Scheduler.h"
#include "ztpHost.h"
#include "ztpRouter.h"



ztpRouter::ztpRouter(Address a,int q_size):FIFONode(a,q_size)
{
	TRACE(TRL3, "Initialized router with address %d\n", a);
}


ztpRouter::~ztpRouter()
{
    // Empty
}
