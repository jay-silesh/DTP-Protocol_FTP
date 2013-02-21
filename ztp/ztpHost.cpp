#include "../netsim/common.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/Packet.h"
#include "ztpPacket.h"
#include "../netsim/Timer.h"
#include "../netsim/PacketScheduler.h"
#include "../netsim/Scheduler.h"
#include "ztpHost.h"



ztpHost::ztpHost(Address a) : FIFONode(a,1100)
{
    TRACE(TRL3, "Created a new Host with address %d\n", a);

}

void
ztpHost::receive(Packet* pkt)
{
    ((ztpPacket*) pkt)->print();
  


    if(((ztpPacket*) pkt)->syn==true && ((ztpPacket*) pkt)->ack==false && ((ztpPacket*) pkt)->fin==false)// && ((ztpPacket*) pkt)->id==0)
    {
      TRACE(TRL1, "\n\n--------------------------------Received the SYN packet\nid of the packet is:%d\nCurrently in the node : %d--------------------------------\n\n",((ztpPacket*) pkt)->id,address());
      destination=((ztpPacket*) pkt)->source;
      normal_packet=true;
      syn_recieved=false;
      finish_packet=true;      
      set_timer(scheduler->time() +0, NULL);
    }
    else if( ((ztpPacket*) pkt)->syn==1 && ((ztpPacket*) pkt)->ack==1  && ((ztpPacket*) pkt)->fin==0 )
    {
      TRACE(TRL1, "\n\n--------------------------------Received the ACK-SYN packet\nid of the packet is:%d\nCurrently in the node : %d--------------------------------\n\n",((ztpPacket*) pkt)->id,address());
      normal_packet=false;
      syn_recieved=true;
      finish_packet=false;    
      set_timer(scheduler->time() +0, NULL);
    }
    else if(((ztpPacket*) pkt)->syn==0 && ((ztpPacket*) pkt)->ack==1 && ((ztpPacket*) pkt)->fin==0)
    {
      TRACE(TRL1,"\n\n-------------Phase 2:three way handshake\n");     
      TRACE(TRL1, "id of the packet is:%d\nCurrently in the node : %d--------------------------------\n\n",((ztpPacket*) pkt)->id,address());
      //set_timer(scheduler->time() +0, NULL);
    }
    else if(((ztpPacket*) pkt)->syn==0 && ((ztpPacket*) pkt)->ack==0 && ((ztpPacket*) pkt)->fin==0)
    {
      TRACE(TRL1, "\n\n--------------------------------Received the Normal packet\nid of the packet is:%d\nCurrently in the node : %d--------------------------------\n\n",((ztpPacket*) pkt)->id,address());
      //Reveiceved a normal packet...
    }
    else if(((ztpPacket*) pkt)->syn==1 && ((ztpPacket*) pkt)->ack==0 && ((ztpPacket*) pkt)->fin==1)
    {
      TRACE(TRL1, "\n\n--------------------------------Received the FIN packet\nid of the packet is:%d\nCurrently in the node : %d--------------------------------\n\n",((ztpPacket*) pkt)->id,address());
      normal_packet=false;
      syn_recieved=true;
      finish_packet=true;
      set_timer(scheduler->time() +0, NULL);
    }
    else if(((ztpPacket*) pkt)->syn==0 && ((ztpPacket*) pkt)->ack==1 && ((ztpPacket*) pkt)->fin==1)
    {
      TRACE(TRL1, "\n\n--------------------------------Received the FIN-ACK packet\nid of the packet is:%d\nCurrently in the node : %d--------------------------------\n\n",((ztpPacket*) pkt)->id,address());
      normal_packet=true;
      syn_recieved=true;
      finish_packet=true;
      set_timer(scheduler->time() +0, NULL);
    }
    else if(((ztpPacket*) pkt)->syn==1 && ((ztpPacket*) pkt)->ack==1 && ((ztpPacket*) pkt)->fin==1)
    {
      TRACE(TRL1, "\n\n--------------------------------DONE TRANSMISSION!!!\nid of the packet is:%d\nCurrently in the node : %d--------------------------------\n\n",((ztpPacket*) pkt)->id,address());
      //DONE!!
    }
  

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
    pkt->id = sent_so_far;
    


    if(normal_packet==false && finish_packet==false && syn_recieved==false )//&& !(pkt->syn) && !(pkt->ack))
    {
      TRACE(TRL1, "Entering the loop in SYN in the host function\n"); 
      pkt->syn=1;
      pkt->ack=0;
      pkt->fin=0;
      pkt->id=0;
      pkt->data=NULL;  
    }
    else if(syn_recieved==false && normal_packet==true && finish_packet==true)
    {
      
      TRACE(TRL1, "Entering the loop in ACK in the Server\n"); 
      pkt->syn=1; 
      pkt->ack=1;
      pkt->fin=0;
      pkt->id=0;
      pkt->data=NULL;
    }
    else if(syn_recieved==true && normal_packet==false && finish_packet==false )
    {
      pkt->syn=0; 
      pkt->ack=1;
      pkt->fin=0;
      pkt->id=0;
      pkt->data=NULL;
      normal_packet=true;
      syn_recieved=true;
      finish_packet=false;
    }   
    else if(syn_recieved==true && normal_packet==true && finish_packet==false)
    {
      pkt->syn=0;
      pkt->ack=0;
      pkt->fin=0;
      pkt->id=sent_so_far;
      pkt->data=NULL;  
    }
    else if( normal_packet==false && finish_packet==true && syn_recieved==false)
    {
      pkt->syn=1;
      pkt->ack=0;
      pkt->fin=1;
      pkt->id=99;
      pkt->data=NULL;
    }
    else if(normal_packet==false && finish_packet==true && syn_recieved==true)
    {
      pkt->syn=0;
      pkt->ack=1;
      pkt->fin=1;
      pkt->id=100;
      pkt->data=NULL;
    }
    else if(normal_packet==true && finish_packet==true && syn_recieved==true)
    {
      pkt->syn=1;
      pkt->ack=1;
      pkt->fin=1;
      pkt->id=101;
      pkt->data=NULL;
    }

     
    if (send(pkt)) {
        TRACE(TRL3, "Sent packet from %d, id %d, length %d\n", 
              address(), sent_so_far, (int) PAYLOAD_SIZE);
    }

  if(normal_packet==true && finish_packet==false)
  {
      sent_so_far++;  
      if( (sent_so_far <=packets_to_send))
      {
        set_timer(scheduler->time(), NULL);
      }
      else
      {
        normal_packet=false;
        finish_packet=true;
        syn_recieved=false;
        set_timer(scheduler->time(), NULL);
      }
  }
  
  return;
}

void ztpHost::FDTP(Address s,Address d,Time start_time,char *p)
{
         start = start_time;
         destination = (d);
         sent_so_far = 0;
         file_holder=(p);
         packets_to_send=10;         
         set_timer(start_time, NULL);
}
