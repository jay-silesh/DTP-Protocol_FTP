#include "../netsim/common.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/Packet.h"
#include "CbrPacket.h"
#include "../netsim/Timer.h"
#include "../netsim/PacketScheduler.h"
#include "../netsim/Scheduler.h"

void
ztpPacket::print()
{
    Packet::print_header();
    Packet::print_payload((char *) &data[0], length - sizeof(Packet), false);
}

bool ztpPacket::check_syn(ztpPacketn *temp)
{
	if(temp->syn==true && temp->asyn==false)
		return true;
	return false;
}
bool ztpPacket::check_syn(ztpPacketn *temp)
{
	if(temp->asyn==true && temp->syn==false)
		return true;
	return false;
}

bool ztpPacket::check_packet(ztpPacketn *temp)
{
	if(temp->syn==false && temp->asyn==false && temp->fin==false)
		return true;
	return false;
}

bool ztpPacket::check_fin(ztpPacketn *temp)
{
	if(temp->fin==true)
		return true;
	return false;
}
