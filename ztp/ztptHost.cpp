#include "../netsim/common.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/Packet.h"
#include "CbrPacket.h"
#include "../netsim/Timer.h"
#include "../netsim/PacketScheduler.h"
#include "../netsim/Scheduler.h"
#include "CbrSender.h"

ztpHost::ztpHost{Address a}:FIFONode(a,0)
{
}

ztpHost::~ztpHost()
{

}