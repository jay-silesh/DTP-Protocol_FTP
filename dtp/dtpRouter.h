class Packet;
class Node;

//
// As its name suggests, PacketQueue_router maintains a FIFO queue of
// packets. At each FIFONode, there is one such queue for every
// nexthop. The queue size is configurable.
//

class PacketQueue_router {
    friend class dtpRouter;
    
 public:
    PacketQueue_router(int sz, Address s, Address d);
    ~PacketQueue_router();

    // Return 0 if no space in queue, else 1
    int enq(Packet* p);			// Pointer to packet to send

    // Return first packet, else NULL
    Packet* deq();

	public: 
    queue<Packet*>	packet_queue;	// Queue of packets
    int			max_size;	// Max queue size
    int 		pending_send;	// Set if packet is in transit
    Address		node;		// Node at which queue resides
    Address		neighbor;	// Neighbor for queue
};

typedef map<Address, PacketQueue_router*, ltAddress> QueueMap_router;
typedef map<Address, PacketQueue_router*, ltAddress>::iterator QueueMap_routerIterator;
typedef pair<Address, PacketQueue_router*> QueueMap_routerPair;

//
// The FIFONode implements a FCFS router. Most routers in the Internet
// employ this queuing discipline. For each neighbor, it maintains a
// PacketQueue_router.
//
// The CbrSender class is derived from FIFONode. 
//

class dtpRouter : public Node {
    
 public:
 

    dtpRouter(Address a,int q_size);		// Creating a Router...
    ~dtpRouter();

    // Send a packet: return 0 on failure, 1 on success
    int send(Packet* packet);		// packet to send
    
    // Call when the packet has been sent
    void send_done(Address nhop);	// Neighbor to whom sent
    
    // Receive a packet
    // Needs to be defined by the inheriting class
    void receive(Packet* packet);	// received packet

    // Handle a timer
    // Needs to be defined by the inheriting class
    void handle_timer(void* cookie);

    // Send a packet to a nexthop
    void send_it(Address nhop);
    public:
    int		max_queue_size;			// Number of packets in queue
    QueueMap_router	queue_map;			// Map of queues per nexthop
    int		pending_send;			// Boolean
};

