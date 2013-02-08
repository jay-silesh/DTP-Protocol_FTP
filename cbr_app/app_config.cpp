#include <string.h>
#include "../netsim/common.h"
#include "../netsim/Config.h"
#include "../netsim/Node.h"
#include "../netsim/FIFONode.h"
#include "../netsim/Packet.h"
#include "CbrSender.h"
#include "CbrReceiver.h"

void
Config::process_app_command(char* id)
{
    // Insert app-level commands here
    if (strcmp(id, "CbrSender") == 0) {
        if (config_argnum != 5) {
            FATAL("Incorrect number of args for: %s", id);
        }
        CbrSender* cs = new CbrSender((Address) config_args[0].numval,
                                      (Address) config_args[1].numval,
                                      (Time) config_args[2].numval,
                                      (Time) config_args[3].numval,
                                      config_args[4].numval);
    } else if (strcmp(id, "CbrReceiver") == 0) {
        if (config_argnum != 1) {
            FATAL("Incorrect number of args for: %s", id);
        }
        CbrReceiver* cr = new CbrReceiver((Address) config_args[0].numval);
    }
    config_argnum = 0;
    return;
}
