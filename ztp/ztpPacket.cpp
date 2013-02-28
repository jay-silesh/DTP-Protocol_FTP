#include "../netsim/common.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/Packet.h"
#include "ztpPacket.h"
#include "../netsim/Timer.h"
#include "../netsim/PacketScheduler.h"
#include "../netsim/Scheduler.h"
#include "string.h"

void
ztpPacket::print()
{
    //Packet::print_header();
   // Packet::print_payload((char *) &data[0], length - sizeof(Packet), false);
    TRACE(TRL1, "SYN %d, ACK %d, FIN: %d,SRC: %d, DST: %d and PCKID %d\n",this->syn,this->ack,this->fin,this->source,this->destination,this->id);
  
}

ztpPacket::ztpPacket( ztpPacket& pck )
{
//	ztpPacket * temp=new ztpPacket();
	this->id=pck.id;
	this->syn=pck.syn;
	this->ack=pck.ack;
	this->length=pck.length;	
	this->fin=pck.fin;
	this->source=pck.source;
	this->destination=pck.destination;
	if(pck.data!=NULL)
	{
		char *buffer=(char *)malloc(1550);
	    strcpy(buffer,pck.data);
		this->data=buffer;
	}
	else
		this->data=NULL;
}

ztpPacket::ztpPacket()
{
}

