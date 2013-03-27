#include "cookie_class.h"


cookie_class::cookie_class()
{

		new_connection=false;
}
cookie_class::cookie_class(int x,all_cookie_states state_type)
{
	id=x;
	cookie_state=state_type;
	new_connection=false;
}

cookie_class::cookie_class(all_cookie_states state_type)
{	
	id=0;
	cookie_state=state_type;
	new_connection=false;
}


cookie_class::~cookie_class()
{

	
}
