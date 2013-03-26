class cookie_class {
 public:
     int id;
     enum all_cookie_states
     {
     	normal,retransmission
     } cookie_state;
     cookie_class();
     cookie_class(int,all_cookie_states);
     ~cookie_class();
};
