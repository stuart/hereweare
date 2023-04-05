// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <systemd/sd-daemon.h>
#include <getopt.h> 

#include "options.h"
#include "scan.h"
#include "http_client.h"

int must_reload_config = 0;
int must_shutdown = 0;

void handle_sigterm(int __attribute__((unused)) signal)
{
    sd_notify(0, "STOPPING=1");
    must_shutdown = 1;
}

void handle_sighup(int __attribute__((unused)) signal)
{
    sd_notify(0, "RELOADING=1");
    must_reload_config = 1;
}

void usage()
{
    printf("Usage: hereweare [-c config_file]\n");
}

int main(int argc, char **argv)
{
    int opt = 0;
    char *config_filename = NULL;
    int activity = 0;

    while ((opt = getopt(argc, argv, "c:h")) != -1)
    {
        switch (opt)
        {
        case 'c':
            config_filename = optarg;
            break;
        case 'h':
            usage();
            exit(EXIT_SUCCESS);
            break;
        case '?':
            if (optopt == 'c')
            {
                fprintf(stderr, "No file name specified for the -c <config file name> option.\n");
                usage();
                exit(EXIT_FAILURE);
            }
            else
            {
                fprintf(stderr, "Invalid option received\n");
                usage();
                exit(EXIT_FAILURE);
            }
            break;
        }
    }

    if (config_filename == NULL)
    {
        config_filename = DEFAULT_CONFIG_FILE;
    }

    struct options opts = load_config(config_filename);
 
    fprintf(stderr, "Started hereweare.\n");
    print_options(stderr, &opts);

    signal(SIGTERM, handle_sigterm);
    signal(SIGHUP, handle_sighup);
    sd_notify(0, "READY=1");

    while (must_shutdown == 0)
    {
        if (must_reload_config)
        {
            free_opts(&opts);
            opts = load_config(config_filename);
            must_reload_config = 0;
            fprintf(stderr, "Reloaded config.\n");
            print_options(stderr, &opts);
        }

        if (scan_devices(&opts))
        {
            printf("Activity detected\n");
            if (activity == 0)
            {
                activity_detected(&opts);
                activity = 1;
            }
            sleep(opts.active_wait_time);
        }
        else
        {
             printf("Inctivity detected\n");
            if (activity == 1)
            {
                inactivity_detected(&opts);
                activity = 0;
            }
            sleep(opts.inactive_wait_time);
        }
    }

    free_opts(&opts);
    exit(EXIT_SUCCESS);
}
