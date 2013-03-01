#include "../netsim/common.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/Packet.h"
#include "dtpPacket.h"
#include "../netsim/Timer.h"
#include "../netsim/PacketScheduler.h"
#include "../netsim/Scheduler.h"
#include "string.h"

void
dtpPacket::print()
{
    //Packet::print_header();
   // Packet::print_payload((char *) &data[0], length - sizeof(Packet), false);
    TRACE(TRL1, "SYN %d, ACK %d, FIN: %d,SRC: %d, DST: %d and PCKID %d\n",this->syn,this->ack,this->fin,this->source,this->destination,this->id);
  
}

dtpPacket::dtpPacket( dtpPacket& pck )
{
	this->id=pck.id;
	this->syn=pck.syn;
	this->ack=pck.ack;
	this->length=pck.length;	
	this->fin=pck.fin;
	this->source=pck.source;
	this->destination=pck.destination;
	this->last_packet=pck.last_packet;
	if(pck.data!=NULL)
	{
		char *buffer=(char *)malloc(1550);
	    strcpy(buffer,pck.data);
		this->data=buffer;
	}
	else
		this->data=NULL;
	
}

dtpPacket::dtpPacket()
{
}

