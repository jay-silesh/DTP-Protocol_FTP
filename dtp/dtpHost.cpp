#include "../netsim/common.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/Packet.h"
#include "dtpPacket.h"
#include "../netsim/Timer.h"
#include "../netsim/PacketScheduler.h"
#include "../netsim/Scheduler.h"
#include "dtpHost.h"

#include <iostream>
#include <string.h>
#include <fstream>
#include <cstring>
#include "cookie_class.h"



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

dtpHost::dtpHost(Address a) : FIFONode(a,16000)
{
  TRACE(TRL3, "Initialized host with address %d\n", a);
  state=dtpHost::SYN;
  sender=false;
}



void
dtpHost::receive(Packet* pkt)
{
    ((dtpPacket*) pkt)->print();  
    
    if(((dtpPacket*) pkt)->syn==true && ((dtpPacket*) pkt)->ack==false && ((dtpPacket*) pkt)->fin==false)// && ((dtpPacket*) pkt)->id==0)
    {
      //  Received the SYN packet
      destination=((dtpPacket*) pkt)->source;
      set_timer(scheduler->time(), NULL);
    }
    
    else if( ((dtpPacket*) pkt)->syn==1 && ((dtpPacket*) pkt)->ack==1  && ((dtpPacket*) pkt)->fin==0 )
    {
      //Received the ACK packet
      state=dtpHost::SYN_ACK;
      set_timer(scheduler->time(), NULL);

    }
    
    else if(((dtpPacket*) pkt)->syn==0 && ((dtpPacket*) pkt)->ack==1 && ((dtpPacket*) pkt)->fin==0)
    {
      TRACE(TRL3, "Established FDTP flow from %d to %d (%d)\n", address(), pkt->destination,scheduler->time());
    }
    
    else if(((dtpPacket*) pkt)->syn==0 && ((dtpPacket*) pkt)->ack==0 && ((dtpPacket*) pkt)->fin==0 && ((dtpPacket*) pkt)->id!=0)
    {
      //Received the Normal packet\nid of the packet
      if(address()==2)
        writing(((dtpPacket*) pkt)->data);
      else
        writing2(((dtpPacket*) pkt)->data);
    }

    else if(((dtpPacket*) pkt)->syn==1 && ((dtpPacket*) pkt)->ack==0 && ((dtpPacket*) pkt)->fin==1)
    {
     //Received the FIN packet
      state=dtpHost::FIN;
      set_timer(scheduler->time(), NULL);
    }
    
    else if(((dtpPacket*) pkt)->syn==0 && ((dtpPacket*) pkt)->ack==1 && ((dtpPacket*) pkt)->fin==1)
    {
    //Received the FIN-ACK packet
      state=dtpHost::FIN_ACK;
      set_timer(scheduler->time(), NULL);
    }


    else if(((dtpPacket*) pkt)->syn==1 && ((dtpPacket*) pkt)->ack==1 && ((dtpPacket*) pkt)->fin==1)
    {
     // DONE TRANSMISSION!!!
      TRACE(TRL3, "Tore down FDTP flow from %d to %d (%d)\n", address(), pkt->destination, scheduler->time());
    }
    delete pkt;
}

dtpHost::~dtpHost()
{
    // Empty
}


void
dtpHost::handle_timer(void* cookie)
{
    dtpPacket*  pkt = new dtpPacket;
    
    

      pkt->source = address();
      pkt->destination = destination;
      pkt->length = sizeof(Packet);// + PAYLOAD_SIZE;
      pkt->id = sent_so_far;
      
      if(state==dtpHost::SYN)
      {
        if(sender==true)
          set_packet((dtpPacket*) pkt,1,0,0,0);
        else //if it is the reciever
          set_packet((dtpPacket*) pkt,1,1,0,0);
        pkt->data=NULL;
      }
      
     
      else if(state==dtpHost::SYN_ACK)
      {
        //Sending the SYN-ACK packets
        set_packet((dtpPacket*) pkt,0,1,0,1);
        pkt->data=NULL;
        state=dtpHost::sending;  
      }

      else if(state==dtpHost::sending)
      {
        //Sending NORMAL packets
        done_transmission=false;
        set_packet((dtpPacket*) pkt,0,0,0,sent_so_far+1);
        pkt->data=file_handling((pkt->id)-2,file_holder);
        if(strlen(pkt->data)<PAYLOAD_SIZE || pkt->data==NULL)
          done_transmission=true;
        pkt->length+=strlen(pkt->data);
      }

      else if( state==dtpHost::FIN)
      {
        //Sending FIN packets
        //this is at the SENDER SIDE
        if(sender==true)
          set_packet((dtpPacket*) pkt,1,0,1,999);
        else
          set_packet((dtpPacket*) pkt,0,1,1,1000);
        pkt->data=NULL;
      }
      
      else if(state==dtpHost::FIN_ACK)
      {
        //Sending FIN-ACK packet
        //this is at the reciever side
        set_packet((dtpPacket*) pkt,1,1,1,1001);
        pkt->data=NULL;      
      }     
  
  

     
  if (send(pkt)) {
      TRACE(TRL3, "Sent packet from %d, id %d, length %d\n", 
            address(), sent_so_far, (int) PAYLOAD_SIZE);
  }

  if(state==dtpHost::sending)
  {
      sent_so_far++;  
      if(done_transmission==false)
      {
        set_timer(scheduler->time(), NULL);
      }
      else
      {
        state=dtpHost::FIN;
        set_timer(scheduler->time(), NULL);
      }
  }
  
  return;
}

void dtpHost::FDTP(Address s,Address d,Time start_time,char *p)
{
       //  start = start_time;
         destination = (d);
         sent_so_far = 0;
         file_holder=p;
         sender=true;
         blank();
         set_timer(start_time, NULL);
         
}

void dtpHost::set_packet(Packet* pkt_p,bool syn_p,bool ack_p,bool fin_p)
{
  ((dtpPacket*) pkt_p)->syn=syn_p;
  ((dtpPacket*) pkt_p)->ack=ack_p;
  ((dtpPacket*) pkt_p)->fin=fin_p;
}

void dtpHost::set_packet(Packet* pkt_p,bool syn_p,bool ack_p,bool fin_p,unsigned int id_p)
{
  ((dtpPacket*) pkt_p)->syn=syn_p;
  ((dtpPacket*) pkt_p)->ack=ack_p;
  ((dtpPacket*) pkt_p)->fin=fin_p;
  ((dtpPacket*) pkt_p)->id=id_p;
}


