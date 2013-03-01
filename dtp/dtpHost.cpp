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


void dtpHost::delete_retransmission_timmer(int packet_no)
{
   RetransmissionPacketMapIterator iit=re_packet_map.find(packet_no);
   if (iit == re_packet_map.end()) {
        return;
   }

   re_packet_map.erase (iit);
   TRACE(TRL3,"@ NODE %d -DELETING THE PACKET %d in the RETRANMITTIN Q\n",address(),packet_no);
}





void
dtpHost::receive(Packet* pkt)
{
    ((dtpPacket*) pkt)->print();  
    
    if(((dtpPacket*) pkt)->last_packet==true)
    {
      state=dtpHost::FIN;
      set_timer(scheduler->time(), NULL); 
    }
    else if(((dtpPacket*) pkt)->syn==true && ((dtpPacket*) pkt)->ack==false && ((dtpPacket*) pkt)->fin==false)// && ((dtpPacket*) pkt)->id==0)
    {
      //  Received the SYN packet @ the receiver side
      destination=((dtpPacket*) pkt)->source;
      set_timer(scheduler->time(), NULL);
    }
    
    else if( ((dtpPacket*) pkt)->syn==1 && ((dtpPacket*) pkt)->ack==1  && ((dtpPacket*) pkt)->fin==0 )
    {
      //Received the SYN packet @ the Sender side
      delete_retransmission_timmer(pkt->id);
      state=dtpHost::SYN_ACK;
      set_timer(scheduler->time(), NULL);

    }
    
    else if(((dtpPacket*) pkt)->syn==0 && ((dtpPacket*) pkt)->ack==1 && ((dtpPacket*) pkt)->fin==0)
    {
      TRACE(TRL3, "\n\nPACKET ID IS %d\n\n",pkt->id-1);
      delete_retransmission_timmer(pkt->id-1);
      TRACE(TRL3, "Established FDTP flow from %d to %d (%d)\n", destination,address(),scheduler->time());
    }
    
    else if(((dtpPacket*) pkt)->syn==0 && ((dtpPacket*) pkt)->ack==0 && ((dtpPacket*) pkt)->fin==0 && ((dtpPacket*) pkt)->id!=0)
    {
      //Received the Normal packet\nid of the packet
      if(((dtpPacket*) pkt)->data!=NULL)
      {
        if(address()==2)
          writing(((dtpPacket*) pkt)->data);
        else
          writing2(((dtpPacket*) pkt)->data);
      }
      if(strlen(((dtpPacket*) pkt)->data)<PAYLOAD_SIZE || ((dtpPacket*) pkt)->data==NULL)
      {          
        set_timer(scheduler->time(), NULL);
        state=dtpHost::START_SENDING_FIN;
      }
      
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
      TRACE(TRL3, "\n\nPACKET ID IS %d\n\n",pkt->id-1);
      delete_retransmission_timmer(pkt->id-1);
      state=dtpHost::FIN_ACK;
      set_timer(scheduler->time(), NULL);
    }


    else if(((dtpPacket*) pkt)->syn==1 && ((dtpPacket*) pkt)->ack==1 && ((dtpPacket*) pkt)->fin==1)
    {
     // DONE TRANSMISSION!!!
      TRACE(TRL3, "\n\nPACKET ID IS %d\n\n",pkt->id-1);
      delete_retransmission_timmer(pkt->id-1);
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
    pkt->last_packet=false;
    
    if(cookie==NULL)
    {
        pkt->source = address();
        pkt->destination = destination;
        pkt->length = sizeof(Packet);// + PAYLOAD_SIZE;
        
        if(state==dtpHost::SYN)
        {
          if(sender==true)
          {  
            set_packet((dtpPacket*) pkt,1,0,0,0);
            set_retransmission_map(pkt);
            TRACE(TRL3,"\n\nQING THE PACKET %d at the node %d\n\n",pkt->id,address());
          }
          else //if it is the reciever
          {  
            set_packet((dtpPacket*) pkt,1,1,0,0);
            set_retransmission_cookie(pkt->id,2000);
            TRACE(TRL3,"\n\nSETTINT THE PACKET FOR Retransmission for the packet %d at the node %d\n\n",pkt->id,address());
            set_retransmission_map(pkt);
            TRACE(TRL3,"\n\nQING THE PACKET %d at the node %d\n\n",pkt->id,address());
          }
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
          if(pkt->data==NULL || strlen(pkt->data)<PAYLOAD_SIZE)
          {
             done_transmission=true;
          }
          else 
            pkt->length+=strlen(pkt->data);
        }

        else if( state==dtpHost::FIN)
        {
          //Sending FIN packets
          //this is at the SENDER SIDE
          pkt->data=NULL;
          if(sender==true)
          {
            set_packet((dtpPacket*) pkt,1,0,1,999);
            
            set_retransmission_cookie(pkt->id,4000);
            TRACE(TRL3,"\n\nSETTINT THE PACKET FOR Retransmission for the packet %d at the node %d\n\n",pkt->id,address());
            set_retransmission_map(pkt);
            TRACE(TRL3,"\n\nQING THE PACKET %d at the node %d\n\n",pkt->id,address());
          }
          else
          {  
            set_packet((dtpPacket*) pkt,0,1,1,1000);
     
            set_retransmission_cookie(pkt->id,4000);
            TRACE(TRL3,"\n\nSETTINT THE PACKET FOR Retransmission for the packet %d at the node %d\n\n",pkt->id,address());
            set_retransmission_map(pkt);
            TRACE(TRL3,"\n\nQING THE PACKET %d at the node %d\n\n",pkt->id,address());
          }
          
        }
        
        else if(state==dtpHost::FIN_ACK)//this is at the reciever side
        {
          //Sending FIN-ACK packet          
          set_packet((dtpPacket*) pkt,1,1,1,1001);
          pkt->data=NULL;      
        }  
        else if(state==dtpHost::START_SENDING_FIN ) 
        {
          pkt->data=NULL;
          pkt->last_packet=true;
        }  
  
    }
    else
    {
         TRACE(TRL3,"\n\nCATCHIN THE RE-PACKET TIMMER\n\n");
         RetransmissionPacketMapIterator iit = re_packet_map.find((int)((cookie_class *)cookie)->id);
         if (iit == re_packet_map.end()) {
              TRACE(TRL3,"\n\nDIDNT FIND THE PACKET %d at the node \n\n",(int)((cookie_class *)cookie)->id,address());
              return;
         }
         else
         {
            TRACE(TRL3,"\n\nDAMN!!\n");

            pkt=(dtpPacket*)((*iit).second);
            set_retransmission_cookie(pkt->id,4000);
            TRACE(TRL3,"\n\n***** REsending the packet  %d and the correct one is %d at the node %d\n\n",(int)((cookie_class *)cookie)->id,pkt->id,address());
            TRACE(TRL3,"\n\nSETTINT THE PACKET FOR Retransmission for the packet %d at the node %d\n\n",pkt->id,address());
          }
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
     
  }
  
  return;
}

void dtpHost::FDTP(Address s,Address d,Time start_time,char *p)
{
         destination = (d);
         sent_so_far = 0;
         file_holder=p;
         sender=true;
         blank();
         set_timer(start_time, NULL);
         
         set_retransmission_cookie(0,4000);
         TRACE(TRL3,"\n\nSETTINT THE PACKET FOR Retransmission for the packet 0 at the node %d\n\n",address());
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


void dtpHost::set_retransmission_map(Packet *pkt_t)
{
            dtpPacket* temp_dtp=new dtpPacket(*(dtpPacket*)pkt_t);
            RetransmissionPacketMapPair entry(pkt_t->id,temp_dtp);
            re_packet_map.insert(entry);
}

void dtpHost::set_retransmission_cookie(unsigned int number,int rtt)
{
            cookie_class* temp_cookie = new cookie_class(number);
            set_timer(scheduler->time()+rtt,temp_cookie);
}



