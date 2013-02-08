#include "../netsim/common.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/Packet.h"
#include "CbrPacket.h"
#include "../netsim/PacketScheduler.h"
#include "../netsim/Timer.h"
#include "../netsim/Scheduler.h"
#include "CbrReceiver.h"

ztpRouter::ztpRouter(Address a,int q_size):FIFONode(a,
													q_size) //sending this as a queue size for the router...
{

}


ztpRouter::~ztpRouter()
{
	//Empty

}
