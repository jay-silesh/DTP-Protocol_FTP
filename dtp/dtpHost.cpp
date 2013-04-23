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

int initial_rtt=50000;

int write_host=10;
int write_host2=4;

int initial_cwnd=1;


char * file_handling(int packet_no,char *file_holder_host);
void writing2(char *buffer);
void writing(char *buffer);
void blank();




dtpHost::dtpHost(Address a) : FIFONode(a,16000)
{
  TRACE(TRL3, "Initialized host with address %d\n", a);
  sender=false;
  packet_expected=1;
  rtt_in_host=initial_rtt;
  cwnd_host=initial_cwnd;
}


void
dtpHost::receive(Packet* pkt)
{
    dtpPacket *dpkt=(dtpPacket*) pkt;
    
    if(dpkt->syn==true && dpkt->ack==false && dpkt->fin==false)
    {
      //Received the SYN packet @ the receiver side
      dpkt->print_receiver();
      destination=dpkt->source;
      set_normal_cookie();
    }
    
    else if( dpkt->syn==1 && dpkt->ack==1  && dpkt->fin==0 )
    {
      //Received the SYN packet @ the Sender side
      dpkt->print_receiver();
      delete_retransmission_timmer(dpkt->id);
      state=dtpHost::SYN_ACK;
      set_normal_cookie();
    }
    
    else if(dpkt->syn==0 && dpkt->ack==1 && dpkt->fin==0)
    {
      dpkt->print_receiver();
      TRACE(TRL3, "Established FDTP flow from %d to %d (%d)\n", destination,address(),scheduler->time());
      delete_retransmission_timmer(dpkt->id);

      dtpHost* temp_sender=(dtpHost*) scheduler->get_node(destination);
      temp_sender->state=dtpHost::sending;      
      RetransmissionPacketMapIterator iit=(temp_sender->re_packet_map).find(dpkt->id);
  /*      if (iit == (temp_sender->re_packet_map).end()) {
        //Do Nothing...
      }
      else
      (temp_sender->re_packet_map).erase (iit);*/

       temp_sender->delete_retransmission_timmer(dpkt->id); 
       temp_sender->set_normal_cookie();
    }
    
    else if(dpkt->syn==0 && dpkt->ack==0 && dpkt->fin==0 )//&& dpkt->id!=0)
    {
        //Received the Normal packet
        //NOTE: What ever changes made here has to be made in the "check_inorder_packets()"
        state=dtpHost::sending_ack_listening; //Setting up the sending state for the receiver...
        if(dpkt->id==packet_expected)
        {
              //Received a Inorder packet...                
                dpkt->print_receiver();    
                normal_packet_received=dpkt->id;    //Setting this parameter so that I can send ACK(the pkt id) for NORMAL properly 
                packet_expected++;
                actual_packets_rec++;

                if(dpkt->data!=NULL)
                {
                            if(address()==write_host)
                                 writing(dpkt->data);
                            else if(address()==write_host2)
                               writing2(dpkt->data);

                }
                
                if(packet_expected < actual_packets_rec)
                {
                  check_inorder_packets();
                }
                send_immediately(0,0,1,packet_expected,rtt_in_host);
        }
        else if(dpkt->id > packet_expected)
        {
            //Received a out of order packet...
            InorderPacketMapIterator iterator_temp=order_packet_map.find(dpkt->id);
            if(iterator_temp==map::end)
            {
              actual_packets_rec++;
              dtpPacket *inorder_temp_packet=new dtpPacket(*dpkt);
              InorderPacketMapPair entry_temp(inorder_temp_packet->id,inorder_temp_packet);
              order_packet_map.insert(entry_temp);
            }
        }
        else
          TRACE(TRL3,"\n\nREceived a REPEATED PACKET HERE\n\n");
        
    }

    else if(dpkt->syn==0 && dpkt->ack==0 && dpkt->fin==1)
    {
       //SENDER's side: Received the ACK FOR THE NORMAL PACKET at the sender's side
        TRACE("\n\nREceived ACK of %d\n\n",dpkt->id);
        lastest_ack_rec=dpkt->id;

        dpkt->fin=0;  //Changing the value of fin so that it is in symetry
        dpkt->print_receiver();
        
        delete_retransmission_timmer(dpkt->id-1);
        dtpHost* temp_sender=(dtpHost*) scheduler->get_node(destination);
        temp_sender->delete_retransmission_timmer(dpkt->id);

        if(done_transmission==false || ((dpkt->last_packet==false)&&(dpkt->id==sent_so_far)) )
        {
           int temp_cong=check_congestion(dpkt);
           state=dtpHost::sending;
           set_normal_cookie();
        }
        else if(done_transmission==true || ((dpkt->last_packet==true)&&(dpkt->id==sent_so_far)) )
        {
              /* this is basically checking if this is the last packet and 
                  here after you have to send a FIN packet */
                state=dtpHost::FIN;
                set_normal_cookie();
        }
              
    }

    else if(dpkt->syn==1 && dpkt->ack==0 && dpkt->fin==1)
    {
       //Received the FIN packet
        dpkt->print_receiver();
        packets_rec=dpkt->id;
        state=dtpHost::FIN;
        set_normal_cookie();
    }
    
    else if(dpkt->syn==0 && dpkt->ack==1 && dpkt->fin==1)
    {
    //Received the FIN-ACK packet
      dpkt->print_receiver();
      packets_rec=dpkt->id;
      delete_retransmission_timmer(dpkt->id-1);
      state=dtpHost::FIN_ACK;
      set_normal_cookie();
    }


    else if(dpkt->syn==1 && dpkt->ack==1 && dpkt->fin==1)
    {
      // DONE TRANSMISSION!!!
      dpkt->print_receiver();
      delete_retransmission_timmer(dpkt->id-1);
      
      dtpHost* temp_sender=(dtpHost*) scheduler->get_node(destination);
      RetransmissionPacketMapIterator iit=(temp_sender->re_packet_map).find(dpkt->id);
      if (iit == (temp_sender->re_packet_map).end()) {
      }
      else
      {
          (temp_sender->re_packet_map).erase (iit);
          TRACE(TRL3, "Tore down FDTP flow from %d to %d (%d)\n", dpkt->source, dpkt->destination, scheduler->time());
      }
    }
    delete pkt;

}

dtpHost::~dtpHost()
{
    // Empty
}


void dtpHost::handle_timer(void* cookie)
{
    dtpPacket*  pkt = new dtpPacket;
    
    cookie_class * new_cookie=(cookie_class*)cookie;

    if(new_cookie->cookie_state==cookie_class::normal)
    {
        
        if(new_cookie->new_connection==true)
        {
          destination = new_cookie->destination;
          sent_so_far = 0;
          file_holder=new_cookie->file_holder;
          sender=true;
          state=dtpHost::SYN;        
          packet_expected=1;
          rtt_in_host=initial_rtt;
          cwnd_host=initial_cwnd;
          done_transmission=false;         

          /* setting up the destination's parameters...*/
          dtpHost* temp_destination=(dtpHost*) scheduler->get_node(destination);
          temp_destination->sent_so_far=0;
          temp_destination->sender=false;
          temp_destination->done_transmission=false;
          temp_destination->state=dtpHost::SYN;
          temp_destination->packet_expected=1;
          temp_destination->rtt_in_host=initial_rtt;
          temp_destination->cwnd_host=initial_cwnd;
          /*********************************************/
        }

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
              set_retransmission_cookie(pkt->id,rtt_in_host);
              set_retransmission_map(pkt);
            }
            else //if it is the reciever
            {  
              set_packet((dtpPacket*)pkt,1,1,0,0);
              set_retransmission_cookie(pkt->id,rtt_in_host);
              set_retransmission_map(pkt);
            }
        }

        else if(state==dtpHost::SYN_ACK)
        {
            //Sending the SYN-ACK packets
            set_packet((dtpPacket*)pkt,0,1,0,0);
            pkt->data=NULL;
            set_retransmission_cookie(pkt->id,rtt_in_host);
            set_retransmission_map(pkt);
        }

        else if(state==dtpHost::sending)
        {
              
              //Sending NORMAL packets
              
              if(sender==true)
              {
                  done_transmission=false;
                  
                  int temp_cwnd=cwnd_host;
                  while( temp_cwnd > 0 && done_transmission==false )
                  {
                        dtpPacket*  pkt_normal = new dtpPacket(address(),destination,sizeof(Packet));
                        set_packet((dtpPacket*)pkt_normal,0,0,0,sent_so_far+1);
                        pkt->data=file_handling((pkt_normal->id)-1,file_holder);
                        
                        set_retransmission_cookie(pkt_normal->id, rtt_in_host*2 );
                        set_retransmission_map(pkt_normal);

                        sent_so_far++;
                        
                        if(pkt_normal->data==NULL)
                        {
                           done_transmission=true;
                           pkt->last_packet=true;
                        }
                        else if ( strlen(pkt_normal->data)<PAYLOAD_SIZE)
                        {
                           pkt_normal->length+=strlen(pkt_normal->data);
                           done_transmission=true;  
                           pkt->last_packet=true;
                        }
                        else
                        {
                           pkt_normal->length+=strlen(pkt_normal->data);
                        }

                        send(pkt_normal);
                        temp_cwnd--;
                  }

                  delete pkt;
                  return;
              }
              else
              {
                  TRACE(TRL3,"\n\n1.ERROR ... SHOULDN't come here...\n\n");

              }

        }
        else if( state==dtpHost::sending_ack_listening)
        {
            
              TRACE(TRL3,"\n\n2.ERROR ... SHOULDN't come here...\n\n");
        }

        else if( state==dtpHost::FIN)          //this is at the SENDER SIDE
        {
            //Sending FIN packets
            pkt->data=NULL;
            if(sender==true)
            {
              set_packet((dtpPacket*)pkt,1,0,1,sent_so_far+1);
              set_retransmission_cookie(pkt->id,rtt_in_host);
              set_retransmission_map(pkt);
            }
            else
            { 
               set_packet((dtpPacket*)pkt,0,1,1,packets_rec+1);
               set_retransmission_cookie(pkt->id,rtt_in_host);
               set_retransmission_map(pkt);
            }
        }
        
        else if(state==dtpHost::FIN_ACK)//this is at the reciever side
        {
            //Sending FIN-ACK packet         
            set_packet((dtpPacket*)pkt,1,1,1,packets_rec+1);
            pkt->data=NULL;      
            set_retransmission_cookie(pkt->id,rtt_in_host);
            set_retransmission_map(pkt);
        }  
        ((dtpPacket*) pkt)->print_sender();
      

      send(pkt);
    }
    else if( new_cookie->cookie_state==cookie_class::retransmission)
    {
        /* This is thr re-transmission part of the code.... 
          The host is now Re-transmitting one of the packets */
          
         dtpPacket * pkt1;
         RetransmissionPacketMapIterator iit = re_packet_map.find(new_cookie->id);
         if (iit == re_packet_map.end()) {
            return;
         }
         else
         {
            dtpPacket *temp=(dtpPacket*)((*iit).second);
            
            pkt1 = new dtpPacket(*temp);
            
            delete_retransmission_timmer(temp->id);
            set_retransmission_cookie(pkt1->id,rtt_in_host*(cwnd_host+1));
            set_retransmission_map(pkt1);
          }
     
        ((dtpPacket*) pkt1)->print_resender();
        send(pkt1);
    }

  
  return;
}

void dtpHost::FDTP(Address s,Address d,Time start_time,char *p)
{
         blank();
         cookie_class* temp_cookie = new cookie_class(cookie_class::normal);
         temp_cookie->destination=d;
         temp_cookie->sent_so_far=0;
         temp_cookie->sender=true;
         temp_cookie->file_holder=p;
         temp_cookie->new_connection=true;
         set_timer(start_time,temp_cookie);
}

void dtpHost::set_packet(Packet* pkt_p,bool syn_p,bool ack_p,bool fin_p)
{
  ((dtpPacket*)pkt_p)->syn=syn_p;
  ((dtpPacket*)pkt_p)->ack=ack_p;
  ((dtpPacket*)pkt_p)->fin=fin_p;
}

int dtpHost::check_congestion(Packet* pkt_p)
{
    cwnd_host*=2;
    return 1;
}


void dtpHost::set_packet(Packet* pkt_p,bool syn_p,bool ack_p,bool fin_p,unsigned int id_p)
{

  ((dtpPacket*)pkt_p)->syn=syn_p;
  ((dtpPacket*)pkt_p)->ack=ack_p;
  ((dtpPacket*)pkt_p)->fin=fin_p;
  ((dtpPacket*)pkt_p)->id=id_p;
}


void dtpHost::set_retransmission_map(const Packet *pkt_t)
{
      //      TRACE(TRL3,"\n\nInserting RE-trnas: %d @ host %d\n\n",pkt_t->id,address());          
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

void dtpHost::send_immediately(bool syn_temp,bool ack_temp,bool fin_temp,unsigned int id_temp,Time time_temp)
{
        dtpPacket * pkt_temp=new dtpPacket(address(),destination,sizeof(Packet));
        pkt_temp->data=NULL;
        set_packet((dtpPacket*)pkt_temp,syn_temp,ack_temp,fin_temp,id_temp);  //sent_so_far+2 = Since ACK shd always be one plus
        set_retransmission_cookie(pkt_temp->id,time_temp);  //Sending the ACK immediately...
        set_retransmission_map(pkt_temp);
        send(pkt_temp);
}

void dtpHost::check_inorder_packets()
{
      InorderPacketMapIterator ott;
      for (ott=order_packet_map.begin(); ott!=order_packet_map.end(); ++ott)
      {
          if(ott->first==packet_expected)
          {
                dtpPacket *dpkt=(dtpPacket*)((*ott).second);
                    
                dpkt->print_receiver();    
                normal_packet_received=dpkt->id;    //Setting this parameter so that I can send ACK(the pkt id) for NORMAL properly 
                packet_expected++;
                
                if(dpkt->data!=NULL)
                {
                            if(address()==write_host)
                                 writing(dpkt->data);
                            else if(address()==write_host2)
                               writing2(dpkt->data);

                }
                
                
               order_packet_map.erase(ott);
          }
          else
          { 
            break;
          }
      }
      
}



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


void writing(char *buffer)
{
  //TRACE(TRL3,"\n\nWRITING -> %d\n\n",++wcounter);
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

