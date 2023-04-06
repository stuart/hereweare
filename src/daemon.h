#pragma once

/* Create a sysV type daemon. Used for non systemd systems.*/
#define BD_NO_CLOSE_FILES    1 /* Don't close all open files */
#define BD_NO_REOPEN_STD_FDS 2 /* Don't reopen stdin, stdout, and stderr to /dev/null */
#define BD_NO_RESET_SIGNALS  4 /* Dont'r reset all the signal handlers */
#define BD_MAX_CLOSE       8192 /* Max file descriptors to close if sysconf(_SC_OPEN_MAX) is indeterminate */

int daemonize(int flags);
