#include "../netsim/common.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/Packet.h"
#include "CbrPacket.h"
#include "../netsim/Timer.h"
#include "../netsim/PacketScheduler.h"
#include "../netsim/Scheduler.h"
#include "CbrSender.h"

CbrSender::CbrSender(Address a,
                     Address d,
                     Time s,
                     Time ip,
                     int npk) : FIFONode(a,40)
{
    start = s;
    destination = d;
    inter_packet_time = ip;
    packets_to_send = npk;
    sent_so_far = 0;

    set_timer(s, NULL);
    TRACE(TRL3, "Created a new CBR sender address %d, target %d\n", a, d);
}

CbrSender::~CbrSender()
{
    // Empty
}

void
CbrSender::handle_timer(void* cookie)
{
    CbrPacket*	pkt = new CbrPacket;
    unsigned char* d = &(pkt->data[0]);

    pkt->source = address();
    pkt->destination = destination;
    pkt->length = sizeof(Packet) + PAYLOAD_SIZE;
    pkt->id = sent_so_far;
    for (int i = 0; i < PAYLOAD_SIZE; i++) {
        *(d+i) = (unsigned char) i;	// Write stuff to the payload
    }

    if (send(pkt)) {
        TRACE(TRL3, "Sent packet from CBR sender at %d, id %d, length %d\n", 
              address(), sent_so_far, (int) PAYLOAD_SIZE);
    }

    sent_so_far++;
    if (sent_so_far < packets_to_send) {
        set_timer(scheduler->time() + inter_packet_time, NULL);
    }
    return;
}
