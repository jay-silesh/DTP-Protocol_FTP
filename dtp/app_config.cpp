#include <string.h>
#include "../netsim/common.h"
#include "../netsim/Config.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/Packet.h"
#include "dtpHost.h"
#include "../netsim/PacketScheduler.h"
#include "dtpPacket.h"
#include "../netsim/Timer.h"
#include "../netsim/Scheduler.h"
#include "dtpRouter.h"




void
Config::process_app_command(char* id)
{
    // Insert app-level commands here
    if (strcmp(id, "Host") == 0) {
        if (config_argnum != 1) {
            FATAL("Incorrect number of args for: %s", id);
        }
        
        dtpHost* zHost = new dtpHost((Address) config_args[0].numval);
    
    } else if (strcmp(id, "Router") == 0) {
        
       if (config_argnum != 2) {
            FATAL("Incorrect number of args for: %s", id);
        }
        
         dtpRouter * zRouter=new dtpRouter((Address)config_args[0].numval,(int)config_args[1].numval);
        
    }
    else if (strcmp(id, "FDTPFlow") == 0) {
        if (config_argnum != 4) {
            FATAL("Incorrect number of args for: %s", id);
        }
         
        dtpHost* zsender=(dtpHost*) scheduler->get_node((Address)config_args[0].numval);
        zsender->FDTP((Address)config_args[0].numval,(Address)config_args[1].numval,(Time)config_args[2].numval,config_args[3].strval);


    }
    config_argnum = 0;
    return;
}
