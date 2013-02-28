#include "../netsim/common.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/Packet.h"
#include "dtpPacket.h"
#include "../netsim/Timer.h"
#include "../netsim/PacketScheduler.h"
#include "../netsim/Scheduler.h"
#include "dtpHost.h"
#include "dtpRouter.h"



dtpRouter::dtpRouter(Address a,int q_size):FIFONode(a,q_size)
{
	TRACE(TRL3, "Initialized router with address %d\n", a);
}


dtpRouter::~dtpRouter()
{
    // Empty
}
