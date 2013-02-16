#include "../netsim/common.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/Packet.h"
#include "ztpPacket.h"
#include "../netsim/Timer.h"
#include "../netsim/PacketScheduler.h"
#include "../netsim/Scheduler.h"
#include "ztpSender.h"

ztpSender::ztpSender(Address a,
                     Address d,
                     Time s,
                     char *file_name) : FIFONode(a,40)
{
    start = s;
    destination = d;
    sent_so_far=0;
    set_timer(s, NULL);
    TRACE(TRL3, "Created a new CBR sender address %d, target %d\n", a, d);
}

ztpSender::~ztpSender()
{
    // Empty
}

void
ztpSender::handle_timer(void* cookie)
{
    ztpPacket*	pkt = new ztpPacket;
    pkt->source = address();
    pkt->destination = destination;
    pkt->length = sizeof(Packet) + PAYLOAD_SIZE;
    pkt->id = sent_so_far;
    pkt->syn=1;


    if (send(pkt)) {
        TRACE(TRL3, "Sent packet from CBR sender at %d, id %d, length %d\n", 
              address(), sent_so_far, (int) PAYLOAD_SIZE);
    }

    sent_so_far++;
/*    if (sent_so_far < packets_to_send) {
        set_timer(scheduler->time() + inter_packet_time, NULL);
    }*/

        
    return;
}
