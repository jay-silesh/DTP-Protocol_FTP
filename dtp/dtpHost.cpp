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


int xxx=2;

void
 blank()
{
  ofstream out("output.txt", ios::out);
  if(!out) {
   TRACE(TRL1, "File Error\n");
  }
  out.close();
  
  ofstream out1("output1.txt", ios::out);
  if(!out1) {
    TRACE(TRL1, "File Error\n");
  }
  out1.close();

}

int wcounter=0;

void writing(char *buffer)
{
  TRACE(TRL3, "\n\n\nWriting.... %d\n\n",++wcounter);
  FILE * pFile;
  pFile = fopen ( "output.txt" , "ab+" );
 // fputs (buffer,pFile);
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
    dtpPacket *dpkt=(dtpPacket*) pkt;
    dpkt->print_receiver();  
    
    if(dpkt->syn==true && dpkt->ack==false && dpkt->fin==false)
    {
      //  Received the SYN packet @ the receiver side
      destination=dpkt->source;
    
      if(xxx==1)
       set_timer(scheduler->time(), NULL);
      else if(xxx=2)
        set_normal_cookie();
    }
    
    else if( dpkt->syn==1 && dpkt->ack==1  && dpkt->fin==0 )
    {
      //Received the SYN packet @ the Sender side
      delete_retransmission_timmer(pkt->id);
      state=dtpHost::SYN_ACK;
      if(xxx==1)
       set_timer(scheduler->time(), NULL);
      else if(xxx=2)
        set_normal_cookie();
    }
    
    else if(dpkt->syn==0 && dpkt->ack==1 && dpkt->fin==0)
    {
      TRACE(TRL3, "Established FDTP flow from %d to %d (%d)\n", destination,address(),scheduler->time());
      delete_retransmission_timmer(pkt->id);

      dtpHost* temp_sender=(dtpHost*) scheduler->get_node(destination);
      temp_sender->state=dtpHost::sending;      
      RetransmissionPacketMapIterator iit=(temp_sender->re_packet_map).find(pkt->id);
      if (iit == (temp_sender->re_packet_map).end()) {
        //Do Nothing...
      }
      else
      (temp_sender->re_packet_map).erase (iit);
      
      if(xxx==1)
        temp_sender->set_timer(scheduler->time(), NULL);     
      else if (xxx==2)
        temp_sender->set_normal_cookie();
    }
    
    else if(dpkt->syn==0 && dpkt->ack==0 && dpkt->fin==0 )//&& dpkt->id!=0)
    {
      //Received the Normal packet
      if(dpkt->data!=NULL)
      {
          if(address()==2)
              writing(dpkt->data);
          else if(address()==1)
             writing2(dpkt->data);
          
      }
      if(dpkt->data==NULL || strlen(dpkt->data)<PAYLOAD_SIZE)
      {          
        dtpHost* temp_sender=(dtpHost*) scheduler->get_node(destination);
        temp_sender->state=dtpHost::FIN;
      
        if(xxx==1)
           temp_sender->set_timer(scheduler->time(), NULL);     
        else if(xxx==2)
          temp_sender->set_normal_cookie();
      }
      
    }

    else if(dpkt->syn==1 && dpkt->ack==0 && dpkt->fin==1)
    {
     //Received the FIN packet
      packets_rec=dpkt->id;
      state=dtpHost::FIN;
      if(xxx==1)
      set_timer(scheduler->time(), NULL);
      else if(xxx==2)
      set_normal_cookie();
    }
    
    else if(dpkt->syn==0 && dpkt->ack==1 && dpkt->fin==1)
    {
    //Received the FIN-ACK packet
      packets_rec=dpkt->id;
      delete_retransmission_timmer(pkt->id-1);
      state=dtpHost::FIN_ACK;
      if(xxx==1)
        set_timer(scheduler->time(), NULL);
      else if(xxx==2)
        set_normal_cookie();
    }


    else if(dpkt->syn==1 && dpkt->ack==1 && dpkt->fin==1)
    {
     // DONE TRANSMISSION!!!
      delete_retransmission_timmer(pkt->id-1);
      
      dtpHost* temp_sender=(dtpHost*) scheduler->get_node(destination);
      RetransmissionPacketMapIterator iit=(temp_sender->re_packet_map).find(pkt->id);
      if (iit == (temp_sender->re_packet_map).end()) {
      }
      else
      {
          (temp_sender->re_packet_map).erase (iit);
          TRACE(TRL3, "Tore down FDTP flow from %d to %d (%d)\n", pkt->source, pkt->destination, scheduler->time());
          
          //Setting up new_host_setting


      }
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
    
    cookie_class * new_cookie=(cookie_class*)cookie;

//    if(new_cookie==NULL)
    if(new_cookie->cookie_state==cookie_class::normal)
    {
        pkt->source = address();
        pkt->destination = destination;
        pkt->length = sizeof(Packet);// + PAYLOAD_SIZE;
        
        //Set packet parameters... // SYN,ACK,FIN,ID

        if(state==dtpHost::SYN)
        {
            pkt->data=NULL;
            if(sender==true)
            {  
              set_packet((dtpPacket*)pkt,1,0,0,0);
              set_retransmission_cookie(pkt->id,4000);
              set_retransmission_map(pkt);
            }
            else //if it is the reciever
            {  
              set_packet((dtpPacket*)pkt,1,1,0,0);
              set_retransmission_cookie(pkt->id,4000);
              set_retransmission_map(pkt);
            }
        }

        else if(state==dtpHost::SYN_ACK)
        {
            //Sending the SYN-ACK packets
            set_packet((dtpPacket*)pkt,0,1,0,0);
            pkt->data=NULL;
            set_retransmission_cookie(pkt->id,4000);
            set_retransmission_map(pkt);
        }

        else if(state==dtpHost::sending)
        {
            //Sending NORMAL packets
            done_transmission=false;
            set_packet((dtpPacket*)pkt,0,0,0,sent_so_far+1);
            pkt->data=file_handling((pkt->id)-1,file_holder);
            sent_so_far++;
            if(pkt->data==NULL || strlen(pkt->data)<PAYLOAD_SIZE)
            {
               done_transmission=true;
            }
            else 
              pkt->length+=strlen(pkt->data);
        }

        else if( state==dtpHost::FIN)          //this is at the SENDER SIDE
        {
            //Sending FIN packets
            pkt->data=NULL;
            if(sender==true)
            {
              set_packet((dtpPacket*)pkt,1,0,1,sent_so_far+1);
              set_retransmission_cookie(pkt->id,4000);
              set_retransmission_map(pkt);
            }
            else
            { 
               set_packet((dtpPacket*)pkt,0,1,1,packets_rec+1);
               set_retransmission_cookie(pkt->id,4000);
               set_retransmission_map(pkt);
            }
        }
        
        else if(state==dtpHost::FIN_ACK)//this is at the reciever side
        {
            //Sending FIN-ACK packet         
            set_packet((dtpPacket*)pkt,1,1,1,packets_rec+1);
            pkt->data=NULL;      
            set_retransmission_cookie(pkt->id,4000);
            set_retransmission_map(pkt);
        }  
        ((dtpPacket*) pkt)->print_sender();
    }
   // else
    else if( new_cookie->cookie_state==cookie_class::retransmission)
    {
         //RetransmissionPacketMapIterator iit = re_packet_map.find((int)((cookie_class *)cookie)->id);
         RetransmissionPacketMapIterator iit = re_packet_map.find(new_cookie->id);
         if (iit == re_packet_map.end()) {
            return;
         }
         else
         {
            dtpPacket *temp=(dtpPacket*)((*iit).second);
            delete pkt;
            
            dtpPacket*  pkt = new dtpPacket(*temp);
            
            delete_retransmission_timmer(temp->id);
            set_retransmission_cookie(pkt->id,4000);
            set_retransmission_map(pkt);
          }
        ((dtpPacket*) pkt)->print_resender();
    }

  send(pkt);
  

  if(state==dtpHost::sending)
  {
      //sent_so_far++;  
      if(done_transmission==false)
      { 
          if(xxx==1)
          set_timer(scheduler->time(), NULL);
          else if(xxx==2)
          set_normal_cookie();
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
         if(xxx==1)
          set_timer(start_time, NULL);
         else if(xxx==2)
           set_normal_cookie();
         
}

void dtpHost::set_packet(Packet* pkt_p,bool syn_p,bool ack_p,bool fin_p)
{
  ((dtpPacket*)pkt_p)->syn=syn_p;
  ((dtpPacket*)pkt_p)->ack=ack_p;
  ((dtpPacket*)pkt_p)->fin=fin_p;
}

void dtpHost::set_packet(Packet* pkt_p,bool syn_p,bool ack_p,bool fin_p,unsigned int id_p)
{

  ((dtpPacket*)pkt_p)->syn=syn_p;
  ((dtpPacket*)pkt_p)->ack=ack_p;
  ((dtpPacket*)pkt_p)->fin=fin_p;
  ((dtpPacket*)pkt_p)->id=id_p;
}


void dtpHost::set_retransmission_map(Packet *pkt_t)
{
                        
            dtpPacket* temp_dtp=new dtpPacket(*(dtpPacket*)pkt_t);
            RetransmissionPacketMapPair entry(pkt_t->id,temp_dtp);
            re_packet_map.insert(entry);
}

void dtpHost::set_retransmission_cookie(unsigned int number,int rtt)
{
            cookie_class* temp_cookie = new cookie_class(number,cookie_class::retransmission);
            set_timer(scheduler->time()+rtt,temp_cookie);
}

void dtpHost::set_normal_cookie()
{
            cookie_class* temp_cookie = new cookie_class(cookie_class::normal);
            set_timer(scheduler->time(),temp_cookie);
}


void dtpHost::delete_retransmission_timmer(int packet_no)
{
   RetransmissionPacketMapIterator iit=re_packet_map.find(packet_no);
   if (iit == re_packet_map.end()) {
        return;
   }

   re_packet_map.erase (iit);
}

