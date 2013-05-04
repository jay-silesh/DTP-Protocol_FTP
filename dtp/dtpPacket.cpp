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
	 //  TRACE(TRL3, "Sending the Packet with the HEADER:source: %d, destination: %d, id: %d, length: %d, SYN %d, ACK %d, FIN: %d\n",(int) source, (int) destination, id, length,this->syn,this->ack,this->fin);
//	  TRACE(TRL3, "Sending the Packet with the HEADER:source: %d, destination: %d, id: %d, length: %d, SYN %d, ACK %d, FIN: %d\n",(int) source, (int) destination, id, length,this->syn,this->ack,this->fin);
  	  TRACE(TRL3, "source: %d, destination: %d, length: %d, sn: %d (%d)\n", (int) source, (int) destination, length, id, scheduler->time());
  	  //	  TRACE(TRL3, "%s",data);
  	//TRACE(TRL3,"buvbdui");
  	//puts(data);
  //	  Packet::print_payload();
    //Packet::print_header();
       print_payload2((char *) &data[0], length - sizeof(dtpPacket), true);
    //TRACE(TRL1, "SYN %d, ACK %d, FIN: %d,SRC: %d, DST: %d and PCKID %d\n",this->syn,this->ack,this->fin,this->source,this->destination,this->id);
}

void
dtpPacket::print_resender()
{
	  // TRACE(TRL3, "Re-sending the Packet with the HEADER:source: %d, destination: %d, id: %d, length: %d, SYN %d, ACK %d, FIN: %d\n",(int) source, (int) destination, id, length,this->syn,this->ack,this->fin);
		TRACE(TRL3, "source: %d, destination: %d, length: %d, sn: %d (%d)\n", (int) source, (int) destination, length, id, scheduler->time());
  	  	 print_payload2((char *) &data[0], length - sizeof(dtpPacket), true);
  	  	//Packet::print_payload();
}


void
dtpPacket::print_receiver()
{
	  // TRACE(TRL3, "Received Packet with the HEADER: source: %d, destination: %d, id: %d, length: %d,SYN %d, ACK %d, FIN: %d\n",(int) source, (int) destination, id, length,this->syn,this->ack,this->fin);
		TRACE(TRL3, "source: %d, destination: %d, length: %d, sn: %d (%d)\n", (int) source, (int) destination, length, id, scheduler->time());
  	  //	Packet::print_payload();
}

void
dtpPacket::print_receiver_app()
{
	  // TRACE(TRL3, "Received Packet with the HEADER: source: %d, destination: %d, id: %d, length: %d,SYN %d, ACK %d, FIN: %d\n",(int) source, (int) destination, id, length,this->syn,this->ack,this->fin);
		TRACE(TRL3, "source: %d, destination: %d, length: %d, sn: %d (%d)\n", (int) source, (int) destination, length, id, scheduler->time());
  	  	print_payload2((char *) &data[0], length - sizeof(dtpPacket), true);
  	  //	Packet::print_payload();
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


void
dtpPacket::print_payload2(char *p,
                      unsigned int len,
                      bool ascii)
{
    if (trace & TRL3) {
        if (!ascii) {
            fprintf(stderr, "\t");
            for (int i = 1; i <= len; i++) {
                fprintf(stderr, "%02x ", (unsigned char) *(p+i-1));
                if (i % 8 == 0) {
                    fprintf(stderr, "\n\t");
                }
            }
        } else {
            for (int i = 0; i < len; i++) {
                fprintf(stderr, "%c", *(p+i));
            }
        }
        fprintf(stderr, "\n");
    }
}

