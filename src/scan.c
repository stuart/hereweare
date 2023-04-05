// SPDX-License-Identifier: MIT

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <event2/event.h>

#include "options.h"
#include "scan.h"

void mouse_callback(evutil_socket_t fd, short what, void *arg){
    int *state = (int *)arg;
    *state = SCAN_STATE_ACTIVE;
}

void keyboard_callback(evutil_socket_t fd, short what, void *arg){
    int *state = (int *)arg;
    *state = SCAN_STATE_ACTIVE;
}

void timeout_callback(evutil_socket_t fd, short what, void *arg){
    int *state = (int *)arg;
    *state = SCAN_STATE_INACTIVE;
}

int scan_devices(struct options *opts)
{
    int state = SCAN_STATE_INACTIVE; 

    struct event_base *base = event_base_new();
    if (!base) {
        fprintf(stderr, "Couldn't get an event_base for libevent.");
        exit(EXIT_FAILURE);
    }

    struct timeval scan_time; 
    scan_time.tv_sec = opts->scan_time;
    scan_time.tv_usec = 0;

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

    struct event *mouse_event = event_new(base, mouse, EV_READ, mouse_callback, &state);
    struct event *keyboard_event = event_new(base, keyboard, EV_READ, keyboard_callback, &state);
    struct event *timeout_event = evtimer_new(base, timeout_callback, &state); 

    event_add(mouse_event, NULL);
    event_add(keyboard_event, NULL);
    evtimer_add(timeout_event, &scan_time); 

    event_base_loop(base, EVLOOP_ONCE);
    
    event_free(mouse_event);
    event_free(keyboard_event);
    event_free(timeout_event);

    close(mouse);
    close(keyboard);
    event_base_free(base);

    return (state);
}
