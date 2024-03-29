#include "common.h"
#include "Node.h"
#include "Packet.h"
#include "PacketScheduler.h"
#include "Timer.h"
#include "Topology.h"
#include "Scheduler.h"

PacketData::PacketData(Node* n,
                       Packet* p,
                       Address nh,
                       bool t)
{
    node = n;
    packet = p;
    nexthop = nh;
    trans = t;
}

PacketData::~PacketData()
{
    // Empty
}

PacketScheduler::PacketScheduler()
{
    // Empty
}   

PacketScheduler::~PacketScheduler()
{
    // empty
}

void
PacketScheduler::send_packet(Packet* packet,
                             Address nexthop,
                             Node* node)
{
    // We don't verify the validity of nexthop or the sender address

    // Simply compute the prop and transmission latency, and schedule
    // for sending
    Time comp = scheduler->time() + 
        topology->link_delay(packet, node->address(), nexthop);
    PacketData* p = new PacketData(node, packet, nexthop, false);
    PacketMapPair entry(comp, p);

    packet_map.insert(entry);

    TRACE(TRL1,
          "Scheduled reception of transmission at node %d time %d for packet id %d from %d to %d, nexthop %d\n",
          node->address(), comp, packet->id,
          packet->source, packet->destination, nexthop);

    // Also need to schedule a send done after the transmission delay
    comp = scheduler->time() + 
        topology->link_trans_delay(packet, node->address(), nexthop);
    p = new PacketData(node, packet, nexthop, true);
    PacketMapPair entry2(comp, p);

    packet_map.insert(entry2);

    TRACE(TRL1,
          "Scheduled completion of transmission at node %d time %d for packet id %d from %d to %d, nexthop %d\n",
          node->address(), comp, packet->id,
          packet->source, packet->destination, nexthop);

    return;
}

void
PacketScheduler::register_to_receive(Node* node,
                    Address addr)
{
    NodeMapPair entry(addr, node);
    node_map.insert(entry);
}

// Return the time of the first event in the queue
Time
PacketScheduler::next_packet()
{
    if (packet_map.empty()) {
        return -1;
    }

    PacketMapIterator head = packet_map.begin();
    return (*head).first;
}

// Process the first event in the queue
void
PacketScheduler::process_packet()
{
    if (packet_map.empty()) {
        return;
    }

    // Get the first entry
    PacketMapIterator head = packet_map.begin();
    PacketData* p = (*head).second;
    
    // Update the simulation virtual time
    scheduler->set_clock((*head).first);

    // Remove the entry
    packet_map.erase(head);

    // Call the receiving node
    NodeMapIterator nmi = node_map.find(p->nexthop);
    if (nmi == node_map.end()) {
        FATAL("No node defined for %d, inconsistent config file?\n",
              p->nexthop);
        // Not reached
    }
    Node* receiver = (*nmi).second;

    if (!p->trans) {
        receiver->receive(p->packet);

        TRACE(TRL1, "Delivered packet id %d from %d to %d to node %d\n",
              p->packet->id, p->packet->source, 
              p->packet->destination, receiver->address());

        delete p;
    } else {
        // Notify the sender that the transmission is complete
        p->node->send_done(receiver->address());

        TRACE(TRL1, "Completed transmission of packet id %d from %d to %d to node %d\n",
              p->packet->id, p->packet->source, 
              p->packet->destination, receiver->address());
        delete p;
    }
    return;
}

Node*
PacketScheduler::get_node(Address a)
{
    NodeMapIterator nmi = node_map.find(a);
    if (nmi == node_map.end()) {
        return NULL;
    }
    Node* nd = (*nmi).second;
    return nd;
}
