#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include "options.h"

struct input_event
{
    struct timeval time;
    unsigned short type;
    unsigned short code;
    unsigned int value;
};

int scan_devices(struct options *opts)
{
    int mouse = open(opts->mouse_device, O_RDONLY);
    if (mouse == -1)
    {
        perror("Open mouse");
        exit(EXIT_FAILURE);
    }

    int keyboard = open(opts->keyboard_device, O_RDONLY);
    if (keyboard == -1)
    {
        perror("Open keyboard");
        exit(EXIT_FAILURE);
    }

    int epoll_fd = epoll_create(2);
    struct epoll_event epoll_ev;
    epoll_ev.data.u32 = 1;
    epoll_ev.events = EPOLLIN;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, mouse, &epoll_ev);
    // epoll_ctl(epoll_fd, EPOLL_CTL_ADD, keyboard, &epoll_ev);

    struct epoll_event *events = calloc(5, sizeof(struct epoll_event));

    int result = epoll_wait(epoll_fd, events, 5, opts->scan_time * 1000);
    if (result == -1)
    {
        perror("epoll_wait");
    }

    close(mouse);
    close(keyboard);

    return (result);
}
