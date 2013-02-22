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
    cout << "Cannot open file.\n";
  }
  out.close();
  
  ofstream out1("output1.txt", ios::out);
  if(!out1) {
    cout << "Cannot open file.\n";
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
  pFile = fopen ( "output1.txt" , "ab+" );
  fwrite (buffer , 1 , strlen(buffer) , pFile);
  fclose (pFile);

}

char * file_handling(int packet_no,char *file_holder_host)
{
  FILE * pFile;
  char *buffer=(char *)malloc(1550);
  size_t result;
 // int packet_size=1500;
  int packet_size=PAYLOAD_SIZE;
  pFile = fopen ( file_holder_host , "rb" );
 
  if (pFile==NULL)
  {
    fputs ("File error",stderr); return NULL;
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

ztpHost::ztpHost(Address a) : FIFONode(a,100)
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
    else if(((ztpPacket*) pkt)->syn==0 && ((ztpPacket*) pkt)->ack==0 && ((ztpPacket*) pkt)->fin==0 && ((ztpPacket*) pkt)->id!=0)
    {
      TRACE(TRL1, "\n\n--------------------------------Received the Normal packet\nid of the packet is:%d\nCurrently in the node : %d--------------------------------\n\n",((ztpPacket*) pkt)->id,address());
      if(address()==2)
        {
          writing(((ztpPacket*) pkt)->data);
          TRACE(TRL1,"\n\n\n\nWRITING IN 2\n\n");
        }
      else
        {
          writing2(((ztpPacket*) pkt)->data);
          TRACE(TRL1,"\n\n\n\nWRITING IN 4\n\n");
        }
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
     // f_writing();
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
    ztpPacket*  pkt = new ztpPacket;
    
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
      //pkt->data=file_handling(0);


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
      pkt->syn=0;
      pkt->ack=0;
      pkt->fin=0;
      pkt->id=sent_so_far;
      pkt->data=file_handling((pkt->id)-1,file_holder);
     // pkt->id=sent_so_far;
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
      TRACE(TRL1, "\n\n\n\n\n\n\nERROR IN THE PACKET!!!!!!!!!\n\n\n\n\n\n");
      TRACE(TRL3, "\n\n\n\n\n\n\nERROR IN THE PACKET!!!!!!!!!\n\n\n\n\n\n");
      exit(0);
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
         packets_to_send=2;         
         blank();
         set_timer(start_time, NULL);
}
