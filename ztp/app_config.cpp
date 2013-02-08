#include <string.h>
#include "../netsim/common.h"
#include "../netsim/Config.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/Packet.h"
#include "ztpHost.h"
#include "ztpRouter.h"

void
Config::process_app_command(char* id)
{
    // Insert app-level commands here
    if (strcmp(id, "Host") == 0) {
        if (config_argnum != 1) {
            FATAL("Incorrect number of args for: %s", id);
        }
        
        ztpHost* zHost = new ztpHost((Address) config_args[0].numval);
    
    } else if (strcmp(id, "Router") == 0) {
        
       if (config_argnum != 2) {
            FATAL("Incorrect number of args for: %s", id);
        }
        
        ztpRouter* zRouter = new ztpRouter((Address) config_args[0].numval,config_args[1].numval);
      
    }
    else if (strcmp(id, "FDTPFlow") == 0) {
        if (config_argnum != 4) {
            FATAL("Incorrect number of args for: %s", id);
        }
        ztpSend* zSend = new ztpSend((Address) config_args[0].numval(Address) config_args[1].numval,(Time) config_args[2].numval,(char) config_args[3]);
        zSend->start_connection();
    }
    config_argnum = 0;
    return;
}
