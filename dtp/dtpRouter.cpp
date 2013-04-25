#include "../netsim/common.h"
#include "../netsim/Node.h"
#include "../netsim/Packet.h"
#include "../netsim/Timer.h"
#include "../netsim/PacketScheduler.h"
#include "../netsim/Scheduler.h"
#include "../netsim/Topology.h"
#include "../netsim/FIFONode.h"
#include "dtpRouter.h"
#include "dtpPacket.h"
#include "dtpHost.h"
#include <set>



PacketQueue_router::PacketQueue_router(int sz,
                         Address s,
                         Address d)
{
    pending_send = 0;
    max_size = sz;
    node = s;
    neighbor = d;
}

PacketQueue_router::~PacketQueue_router()
{
    // Empty
}

int
PacketQueue_router::enq(Packet* p)
{
    if (packet_queue.size() < max_size) {
        packet_queue.push(p);
        TRACE(TRL1, "Queue size at (%d, %d) is %d\n", 
              node, neighbor, (int) packet_queue.size());
        return 1;
    }
    return 0;
}

Packet*
PacketQueue_router::deq()
{
    Packet* p;

    if (packet_queue.empty()) {
        return NULL;
    }
    p = packet_queue.front();
    packet_queue.pop();
    TRACE(TRL1, "Queue size at (%d, %d) is %d\n", 
          node, neighbor, (int) packet_queue.size());
    return p;
}

dtpRouter::dtpRouter(Address a,
                   int q) : Node(a)
{
    max_queue_size = q;
    TRACE(TRL3, "Initialized router with address %d\n", a);
}

dtpRouter::~dtpRouter()
{
    // Empty
}

int
dtpRouter::send(Packet* packet)
{
    Address 	 	nhop;
    PacketQueue_router* 	queue;
    QueueMap_routerIterator	qiter;

    // Compute the nexthop
    nhop = topology->nexthop(address(), packet->destination);

    // Find the queue
    qiter = queue_map.find(nhop);
    if (qiter == queue_map.end()) { // An entry doesn't exist
        queue = new PacketQueue_router(max_queue_size, address(), nhop);
        QueueMap_routerPair np(nhop, queue);
        queue_map.insert(np);
    } else {
        queue = (*qiter).second;
    }
    
    // Check if there is space
    if (queue->enq(packet)) {
        TRACE(TRL2, "Enqueued at %d packet (src %d, dst %d, id %d)\n",
              address(), packet->source, packet->destination, packet->id);
        send_it(nhop);
        return 1;
    }

    TRACE(TRL2, "Queue full at %d, dropped packet (src %d, dst %d, id %d)\n",
          address(), packet->source, packet->destination, packet->id);
    delete packet;
    return 0;
}

void
dtpRouter::send_it(Address nhop)
{
    PacketQueue_router*	queue;
    QueueMap_routerIterator	qiter;
    Packet*		pkt;

    qiter = queue_map.find(nhop);
    ASSERT(qiter != queue_map.end());
    queue = (*qiter).second;

    if (queue->pending_send) {
        return;
    }

    pkt = queue->deq();
    if (pkt) {
        TRACE(TRL2, "Forwarding at %d to nexthop %d packet (src %d, dst %d, id %d)\n",
              address(), nhop, pkt->source, pkt->destination, pkt->id);
        Node::send(pkt, nhop);
        queue->pending_send = 1;
    }
    return;
}

void
dtpRouter::send_done(Address nhop)
{
    PacketQueue_router*	queue;
    QueueMap_routerIterator	qiter;

    qiter = queue_map.find(nhop);
    ASSERT(qiter != queue_map.end());
    queue = (*qiter).second;

    TRACE(TRL2, "Ongoing transmission at %d to nexthop %d completed\n",
          address(), nhop);
    queue->pending_send = 0;
    send_it(nhop);
    return;
}

void
dtpRouter::receive(Packet* pkt)
{
    PacketQueue_router*	queue;
    QueueMap_routerIterator	qiter;
    Address		nhop;

    // If the packet is for us, drop it
    if (pkt->destination == address()) {
        delete pkt;
        return;
    }

    // Otherwise, forward packet onwards
    nhop = topology->nexthop(address(), pkt->destination);

    // Find the queue
    qiter = queue_map.find(nhop);
    if (qiter == queue_map.end()) { // An entry doesn't exist
        queue = new PacketQueue_router(max_queue_size, address(), nhop);
        QueueMap_routerPair np(nhop, queue);
        queue_map.insert(np);
    } else {
        queue = (*qiter).second;
    }


    /****************************************************************************/
    // Calculating the Congestion in the router and setting in the packet....
    	Address temp_node=pkt->source;
        ((queue)->source_holder).insert(temp_node);

    	if(((dtpPacket*)pkt)->last_packet==true)
        {
            set<Address>::iterator it2;
            it2 = ((queue)->source_holder).find (temp_node);
            ((queue)->source_holder).erase (it2);
        }

        int no_of_sources=((queue)->source_holder).size();
        if(no_of_sources<3)
            no_of_sources=3;
    	int each_source=(max_queue_size-2)/no_of_sources;
        if(each_source<1)
            each_source=1;
    	//each_source=each_source-3;
    	TRACE(TRL3,"\n\n\nAssigning CWND %d\n\n\n",each_source);
        ((dtpPacket*)pkt)->cwnd_calculated=each_source;
	/****************************************************************************/
    
    // Check if there is space
    if (queue->enq(pkt)) {
        TRACE(TRL2, "Forwarding at %d nexthop %d packet (src %d, dst %d, id %d)\n",
              address(), nhop, pkt->source, pkt->destination, pkt->id);
        send_it(nhop);
        return;
    }

    TRACE(TRL2, "Queue full at %d, dropped packet (src %d, dst %d, id %d)\n",
          address(), pkt->source, pkt->destination,
          pkt->id);
    delete pkt;
    return;
}

void
dtpRouter::handle_timer(void* cookie)
{
    // Empty
}

