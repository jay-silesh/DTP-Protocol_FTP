#include "../netsim/common.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/Packet.h"
#include "ztpPacket.h"
#include "../netsim/Timer.h"
#include "../netsim/PacketScheduler.h"
#include "../netsim/Scheduler.h"
#include "ztpHost.h"


#include <iostream>
#include <string.h>
#include <fstream>
#include <cstring>



void blank()
{
 ofstream out("output.txt", ios::out);
  if(!out) {
   TRACE(TRL3, "File Error\n");
  }
  out.close();
  
  ofstream out1("output1.txt", ios::out);
  if(!out1) {
    TRACE(TRL3, "File Error\n");
  }
  out1.close();

}

void writing(char *buffer)
{
  FILE * pFile;
  pFile = fopen ( "output.txt" , "ab+" );
  fwrite (buffer , 1 , strlen(buffer) , pFile);
  fclose (pFile);

}


void writing2(char *buffer)
{
  FILE * pFile;
  pFile = fopen( "output1.txt" , "ab+" );
  fwrite (buffer , 1 , strlen(buffer) , pFile);
  fclose (pFile);

}

char * file_handling(int packet_no,char *file_holder_host)
{
  FILE * pFile;
  char *buffer=(char *)malloc(1550);
  size_t result;
  int packet_size=PAYLOAD_SIZE;
  pFile = fopen ( file_holder_host , "rb" );
 
  if (pFile==NULL)
  {
      TRACE(TRL3, "File Error\n");
     return NULL;
  }
  
  
    fseek (pFile , packet_no*packet_size , SEEK_SET);
    
    
    result = fread (buffer,1,packet_size,pFile);
    if (buffer != NULL) 
    {
      buffer[strlen(buffer)]='\0';
      char *buff;
      buff=buffer;

      fclose (pFile);
      return buff;
    }
    else
    {
      fclose (pFile);
      return NULL;    
    }
      

}

ztpHost::ztpHost(Address a) : FIFONode(a,16000)
{
TRACE(TRL3, "Initialized host with address %d\n", a);

}

void
ztpHost::receive(Packet* pkt)
{
    ((ztpPacket*) pkt)->print();
  

    if(((ztpPacket*) pkt)->syn==true && ((ztpPacket*) pkt)->ack==false && ((ztpPacket*) pkt)->fin==false)// && ((ztpPacket*) pkt)->id==0)
    {
//      TRACE(TRL1, "\n\n--------------------------------Received the SYN packet\nid of the packet is:%d\nCurrently in the node : %d--------------------------------\n\n",((ztpPacket*) pkt)->id,address());
      
      destination=((ztpPacket*) pkt)->source;
      normal_packet=true;
      syn_recieved=false;
      finish_packet=true;      
      set_timer(scheduler->time(), NULL);
      
      //
    }
    else if( ((ztpPacket*) pkt)->syn==1 && ((ztpPacket*) pkt)->ack==1  && ((ztpPacket*) pkt)->fin==0 )
    {
  //    TRACE(TRL1, "\n\n--------------------------------Received the ACK-SYN packet\nid of the packet is:%d\nCurrently in the node : %d--------------------------------\n\n",((ztpPacket*) pkt)->id,address());
      normal_packet=false;
      syn_recieved=true;
      finish_packet=false;    
      
      set_timer(scheduler->time(), NULL);

    }
    else if(((ztpPacket*) pkt)->syn==0 && ((ztpPacket*) pkt)->ack==1 && ((ztpPacket*) pkt)->fin==0)
    {
      TRACE(TRL3, "Established FDTP flow from %d to %d (%d)\n", address(), pkt->destination,scheduler->time());
    }
    else if(((ztpPacket*) pkt)->syn==0 && ((ztpPacket*) pkt)->ack==0 && ((ztpPacket*) pkt)->fin==0 && ((ztpPacket*) pkt)->id!=0)
    {
      //TRACE(TRL1, "\n\n--------------------------------Received the Normal packet\nid of the packet is:%d\nCurrently in the node : %d--------------------------------\n\n",((ztpPacket*) pkt)->id,address());
      if(address()==2)
        {
          writing(((ztpPacket*) pkt)->data);
         
        }
      else
        {
          writing2(((ztpPacket*) pkt)->data);
        }
     
    }
    else if(((ztpPacket*) pkt)->syn==1 && ((ztpPacket*) pkt)->ack==0 && ((ztpPacket*) pkt)->fin==1)
    {
     // TRACE(TRL1, "\n\n--------------------------------Received the FIN packet\nid of the packet is:%d\nCurrently in the node : %d--------------------------------\n\n",((ztpPacket*) pkt)->id,address());
      normal_packet=false;
      syn_recieved=true;
      finish_packet=true;
      set_timer(scheduler->time() +0, NULL);
      
      
    }
    else if(((ztpPacket*) pkt)->syn==0 && ((ztpPacket*) pkt)->ack==1 && ((ztpPacket*) pkt)->fin==1)
    {
    //  TRACE(TRL1, "\n\n--------------------------------Received the FIN-ACK packet\nid of the packet is:%d\nCurrently in the node : %d--------------------------------\n\n",((ztpPacket*) pkt)->id,address());
      normal_packet=true;
      syn_recieved=true;
      finish_packet=true;
      set_timer(scheduler->time() +0, NULL);
    }
    else if(((ztpPacket*) pkt)->syn==1 && ((ztpPacket*) pkt)->ack==1 && ((ztpPacket*) pkt)->fin==1)
    {
     // TRACE(TRL1, "\n\n--------------------------------DONE TRANSMISSION!!!\nid of the packet is:%d\nCurrently in the node : %d--------------------------------\n\n",((ztpPacket*) pkt)->id,address());
      TRACE(TRL3, "Tore down FDTP flow from %d to %d (%d)\n", address(), pkt->destination, scheduler->time());
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
    ztpPacket*  pkt = new ztpPacket;
    
    pkt->source = address();
    pkt->destination = destination;
    pkt->length = sizeof(Packet);// + PAYLOAD_SIZE;
    pkt->id = sent_so_far;
    


    if(normal_packet==false && finish_packet==false && syn_recieved==false )//&& !(pkt->syn) && !(pkt->ack))
    {
//      TRACE(TRL1, "Entering the loop in SYN in the host function\n"); 
      pkt->syn=1;
      pkt->ack=0;
      pkt->fin=0;
      pkt->id=-1;
      pkt->data=NULL;


    }
    else if(syn_recieved==false && normal_packet==true && finish_packet==true)
    {
      
  //    TRACE(TRL1, "Entering the loop in ACK in the Server\n"); 
      pkt->syn=1; 
      pkt->ack=1;
      pkt->fin=0;
      pkt->id=0;
      pkt->data=NULL;
    
    }
    else if(syn_recieved==true && normal_packet==false && finish_packet==false )
    {

      //TRACE(TRL1, "Entering the loop in SYN-ACK in the Server\n");
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
      //TRACE(TRL1, "Entering the loop in NORMAL in the Server\n");
      done_transmission=false;
      pkt->syn=0;
      pkt->ack=0;
      pkt->fin=0;
      pkt->id=sent_so_far;
      pkt->data=file_handling((pkt->id)-1,file_holder);
      if(strlen(pkt->data)<PAYLOAD_SIZE || pkt->data==NULL)
        done_transmission=true;
      pkt->length+=strlen(pkt->data);
    

    }

    else if( normal_packet==false && finish_packet==true && syn_recieved==false)
    {
      //TRACE(TRL1, "Entering the loop in FIN in the Server\n");
      pkt->syn=1;
      pkt->ack=0;
      pkt->fin=1;
      pkt->id=99;
      pkt->data=NULL;
    

    }
    else if(normal_packet==false && finish_packet==true && syn_recieved==true)
    {
      //TRACE(TRL1, "Entering the loop in FACK in the Server\n");
      pkt->syn=0;
      pkt->ack=1;
      pkt->fin=1;
      pkt->id=100;
      pkt->data=NULL;
      
    }
    else if(normal_packet==true && finish_packet==true && syn_recieved==true)
    {
      //TRACE(TRL1, "Entering the loop in FIN-ACK in the Server\n");
      pkt->syn=1;
      pkt->ack=1;
      pkt->fin=1;
      pkt->id=101;
      pkt->data=NULL;
    
    }

   if(((ztpPacket*) pkt)->syn==0 && ((ztpPacket*) pkt)->ack==0 && ((ztpPacket*) pkt)->fin==0 && ((ztpPacket*) pkt)->id==0)
   {
      TRACE(TRL3, "ERROR IN THE PACKET!!!!!!!!!\n\n\n\n\n\n");
      exit(0);
   }
  

     
    if (send(pkt)) {
        TRACE(TRL3, "Sent packet from %d, id %d, length %d\n", 
              address(), sent_so_far, (int) PAYLOAD_SIZE);
    }

  if(normal_packet==true && finish_packet==false)
  {
      sent_so_far++;  
      if(done_transmission==false)
      {
        set_timer(scheduler->time(), NULL);

        
      }
      else
      {
        // this section is entered if the FIN packet is basically sent...
        // Basically we are setting the timer for the FIN packet.
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
         file_holder=p;
         cookie_count=-1;
         blank();
         set_timer(start_time, NULL);

}
