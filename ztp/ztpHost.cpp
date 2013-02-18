#include "../netsim/common.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/Packet.h"
#include "ztpPacket.h"
#include "../netsim/Timer.h"
#include "../netsim/PacketScheduler.h"
#include "../netsim/Scheduler.h"
#include "ztpHost.h"



ztpHost::ztpHost(Address a) : FIFONode(a,10)
{
    TRACE(TRL3, "Created a new Host with address %d\n", a);

}

void
ztpHost::receive(Packet* pkt)
{
    ((ztpPacket*) pkt)->print();
    //Sending back the ck packet
    TRACE(TRL1, "\n\nReceived the packet %d\n\n",((ztpPacket*) pkt)->id);

    //Sending the ACK for connection EST...
    if(((ztpPacket*) pkt)->syn==0 && ((ztpPacket*) pkt)->ack==1 && ((ztpPacket*) pkt)->id==0)
    {
      TRACE(TRL1,"\n\n\n--------------------------------\nCurrently in am in the node %d\n--------------------------------\n\n\n",address());
      TRACE(TRL1,"Ending the three way handshake\n");     
      have_to_send_ack=false;
    
    }
    else if(((ztpPacket*) pkt)->syn==1 && ((ztpPacket*) pkt)->ack==0 && ((ztpPacket*) pkt)->fin==0 && ((ztpPacket*) pkt)->id==0)
    { 
      TRACE(TRL1, "Changing the packet to have only ACK in it! and i am in the node %d\n",address()); 

      destination=((ztpPacket*) pkt)->source;
      have_to_send_ack=true;
      TRACE(TRL1,"\n\n\n--------------------------------\nCurrently in am in the node %d\n--------------------------------\n\n\n",address());
      set_timer(scheduler->time() +0, NULL);
      
    }
    

    /*set_timer(scheduler->time() +0, NULL);
    
    if (send(pkt)) {
        TRACE(TRL1, "Sent packet from %d, id %d\n", 
              address(), pkt->id, (int) PAYLOAD_SIZE);
    }*/
  
    delete pkt;
}

ztpHost::~ztpHost()
{
    // Empty
}

void
ztpHost::handle_timer(void* cookie)
{
    ztpPacket*	pkt = new ztpPacket;
    
    pkt->source = address();
    pkt->destination = destination;
    pkt->length = sizeof(Packet) + PAYLOAD_SIZE;
    //pkt->id = sent_so_far;
    
    if( ((pkt->id==0) && (pkt->syn) && (pkt->ack)) || ((pkt->id!=0) && !(pkt->syn) && !(pkt->ack)))
    {
      TRACE(TRL3, "Entering the loop in NORMAL in the host function\n"); 
      pkt->syn=0;
      pkt->ack=1;
      pkt->fin=0;
      pkt->id=1;
      pkt->data=NULL;  
    }
    else if( (pkt->id==0) && (pkt->syn) && !(pkt->ack) && have_to_send_ack==true)
    {
      TRACE(TRL3, "Entering the loop in ACK in the host function\n"); 
      pkt->syn=0;
      pkt->ack=1;
      pkt->fin=0;
      pkt->id=0;
      pkt->data=NULL;  
    }
    else if( (pkt->id==0) && !(pkt->syn) && !(pkt->ack))
    {
      TRACE(TRL3, "Entering the loop in SYN in the host function\n"); 
      pkt->syn=1;
      pkt->ack=0;
      pkt->fin=0;
      pkt->id=0;
      pkt->data=NULL;  
    }
     
    if (send(pkt)) {
        TRACE(TRL3, "Sent packet from %d, id %d, length %d\n", 
              address(), sent_so_far, (int) PAYLOAD_SIZE);
    }

  //if(have_to_send_ack)
   // set_timer(scheduler->time(), NULL);

   /* sent_so_far++;
    if (sent_so_far < packets_to_send) {
        //set_timer(scheduler->time() + inter_packet_time, NULL);
   
    }*/
    return;
}

void ztpHost::FDTP(Address s,Address d,Time start_time,char *p)
{
       //  ztpHost *zsender=get_node(s);
         start = start_time;
         destination = (d);
         sent_so_far = 0;
         file_holder=(p);
     //    packets_to_send=0;         
         set_timer(start_time, NULL);
}