#include "log.h"
#include "config.h"

#ifndef HAVE_SYSTEMD
void init_logging(){
    openlog("hereweare", LOG_CONS|LOG_PID, LOG_DAEMON);
}
#endif