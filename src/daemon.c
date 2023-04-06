#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "daemon.h"

int daemonize(int flags)
{
    int maxfd, fd;
    fflush(stdout);

    switch (fork())
    {
    case -1:
        fflush(stdout);
        return -1;
    case 0:
        fflush(stdout);
        break;
    default:
        fflush(stdout);
        _exit(EXIT_SUCCESS);
    }

    if (setsid() == -1)
        return -1;

    switch (fork())
    {
    case -1:
        return -1;
    case 0:
        break;
    default:
        _exit(EXIT_SUCCESS);
    }

    umask(0);
    chdir("/");

    for (int i = 0; i < _NSIG; i++)
    {
        signal(i, SIG_DFL);
    }

    if (!(flags & BD_NO_CLOSE_FILES))
    {
        maxfd = sysconf(_SC_OPEN_MAX);
        if (maxfd == -1)
            maxfd = BD_MAX_CLOSE;
        for (fd = 0; fd < maxfd; fd++)
            close(fd);
    }

    if (!(flags & BD_NO_REOPEN_STD_FDS))
    {
        close(STDIN_FILENO);

        fd = open("/dev/null", O_RDWR);
        if (fd != STDIN_FILENO)
            return -1;
        if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
            return -2;
        if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
            return -3;
    }

    return 0;
}