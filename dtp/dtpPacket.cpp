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
dtpPacket::print_sender()
{
	   TRACE(TRL3, "Sending the Packet with the HEADER:source: %d, destination: %d, id: %d, length: %d, SYN %d, ACK %d, FIN: %d\n",(int) source, (int) destination, id, length,this->syn,this->ack,this->fin);
    //Packet::print_header();
   // Packet::print_payload((char *) &data[0], length - sizeof(Packet), false);
    //TRACE(TRL1, "SYN %d, ACK %d, FIN: %d,SRC: %d, DST: %d and PCKID %d\n",this->syn,this->ack,this->fin,this->source,this->destination,this->id);
}

void
dtpPacket::print_resender()
{
	   TRACE(TRL3, "Re-sending the Packet with the HEADER:source: %d, destination: %d, id: %d, length: %d, SYN %d, ACK %d, FIN: %d\n",(int) source, (int) destination, id, length,this->syn,this->ack,this->fin);
}


void
dtpPacket::print_receiver()
{
	   TRACE(TRL3, "Received Packet with the HEADER: source: %d, destination: %d, id: %d, length: %d,SYN %d, ACK %d, FIN: %d\n",(int) source, (int) destination, id, length,this->syn,this->ack,this->fin);
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
	if(pck.data!=NULL)
	{
		char *buffer = new char [MTU];
		//char *buffer=(char *)malloc(1550);
	    strcpy(buffer,pck.data);
		this->data=buffer;
	}
	else
		this->data=NULL;


	/* Congestion Window,cummu ACK,etc */
	this->last_packet=pck.last_packet;
	this->cwnd_calculated=pck.cwnd_calculated;
	

	
}


dtpPacket::dtpPacket()
{


}


dtpPacket::dtpPacket ( Address source_temp, Address destination_temp,unsigned int length_temp)
{
	  this->source = source_temp;
      this->destination = destination_temp;
      this->length = length_temp;
}