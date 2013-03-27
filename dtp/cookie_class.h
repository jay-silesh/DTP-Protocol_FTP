class cookie_class {
 public:
     int id;

     //
     bool new_connection;
    unsigned short	destination;	// Target address
    bool    done_transmission;
    int		sent_so_far;		// number sent
    char * file_holder;
    bool sender;    
     //

     enum all_cookie_states
     {
     	normal,retransmission
     } cookie_state;
     cookie_class();
     cookie_class(int,all_cookie_states);
     cookie_class(all_cookie_states);
     ~cookie_class();
};
