// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>
#include <string.h>
#include <errno.h>

#include "options.h"
#include "log.h"

char *get_device_name(const char *device_command)
{
    FILE *fp;
    char *line = NULL;
    char *device_path;
    size_t len = 0;
    size_t trim_len = 0;

    fp = popen(device_command, "r");
    if (fp == NULL)
    {
        return ("");
    }

    while (getline(&line, &len, fp) != -1)
    {
    }

    /* Cut the list of devices at the first \n */
    trim_len = strcspn(line, "\n");
    device_path = calloc(trim_len + 1, sizeof(char));
    if (device_path == NULL)
    {
        HR_LOG_ERR("Out of memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    strncpy(device_path, line, trim_len);
    if (strlen(device_path) > 0)
    {
        printf("Found device: %s\n", device_path);
    }
    else
    {
        printf("Device not found.\n");
    }
    return (device_path);
}

void autoconfigure_mouse(struct options *opts)
{
    HR_LOG_ERR("mouse_device not configured, will try to find it automatically.\n");
    opts->mouse_device = get_device_name("ls /dev/input/by-id/*-event-mouse");
}

void autoconfigure_keyboard(struct options *opts)
{
    HR_LOG_ERR("keyboard_device not configured, will try to find it automatically.\n");
    opts->keyboard_device = get_device_name("ls /dev/input/by-id/*-event-kbd");
}

void print_options(FILE *stream, struct options *opts)
{
    fprintf(stream,
            "Mouse Device: %s\n\
Keyboard Device: %s\n\
Home Assistant URL: %s\n\
Active URL: %s\n\
Inactive URL: %s\n\
Home Assistant Token: %s\n\
Active Wait Time: %d\n\
Inactive Wait Time: %d\n\
Scan Time: %d\n\
Active JSON Data: %s\n\
Inactive JSON Data: %s\n",
            opts->mouse_device,
            opts->keyboard_device,
            opts->home_assistant_url,
            opts->home_assistant_active_url,
            opts->home_assistant_inactive_url,
            opts->home_assistant_token,
            opts->active_wait_time,
            opts->inactive_wait_time,
            opts->scan_time,
            opts->active_json_data,
            opts->inactive_json_data);
}

struct options load_config(char *config_filename)
{
    config_t config;
    struct options opts = {};

    config_init(&config);

    if (config_read_file(&config, config_filename) != CONFIG_TRUE)
    {
        HR_LOG_ERR("Cannot read config file: %s\n", config_filename);
        exit(EXIT_FAILURE);
    }

    if (config_lookup_string(&config, "mouse_device", &opts.mouse_device) == CONFIG_FALSE)
        autoconfigure_mouse(&opts);

    if (config_lookup_string(&config, "keyboard_device", &opts.keyboard_device) == CONFIG_FALSE)
        autoconfigure_keyboard(&opts);

    if (config_lookup_string(&config, "home_assistant_url", &opts.home_assistant_url) == CONFIG_FALSE)
        opts.home_assistant_url = DEFAULT_HOME_ASSISTANT_URL;

    const char *path;
    if (config_lookup_string(&config, "home_assistant_active_path", &path) == CONFIG_FALSE)
    {
        HR_LOG_ERR("Missing home_assistant_active_path in config.\n");
        opts.home_assistant_active_url = (char *)opts.home_assistant_url;
    }
    else
    {
        size_t size = snprintf(NULL, 0, "%s%s", opts.home_assistant_url, path);
        opts.home_assistant_active_url = malloc(size + 1);
        snprintf(opts.home_assistant_active_url, size + 1, "%s%s", opts.home_assistant_url, path);
    }

    if (config_lookup_string(&config, "home_assistant_inactive_path", &path) == CONFIG_FALSE)
    {
        HR_LOG_ERR("Missing home_assistant_inactive_path in config.\n");
        opts.home_assistant_inactive_url = (char *)opts.home_assistant_url;
    }
    else
    {
        size_t size = snprintf(NULL, 0, "%s%s", opts.home_assistant_url, path);
        opts.home_assistant_inactive_url = malloc(size + 1);
        snprintf(opts.home_assistant_inactive_url, size + 1, "%s%s", opts.home_assistant_url, path);
    }

    if (config_lookup_string(&config, "home_assistant_inactive_path", &opts.home_assistant_url) == CONFIG_FALSE)
        opts.home_assistant_url = DEFAULT_HOME_ASSISTANT_URL;
    if (config_lookup_string(&config, "home_assistant_token", &opts.home_assistant_token) == CONFIG_FALSE)
        opts.home_assistant_token = DEFAULT_HOME_ASSISTANT_TOKEN;

    if (config_lookup_int(&config, "active_wait_time", &opts.active_wait_time) == CONFIG_FALSE)
        opts.active_wait_time = DEFAULT_ACTIVE_WAIT_TIME;

    if (config_lookup_int(&config, "inactive_wait_time", &opts.inactive_wait_time) == CONFIG_FALSE)
        opts.active_wait_time = DEFAULT_INACTIVE_WAIT_TIME;

    if (config_lookup_int(&config, "scan_time", &opts.scan_time) == CONFIG_FALSE)
        opts.scan_time = DEFAULT_SCAN_TIME;

    if (config_lookup_string(&config, "home_assistant_active_json_data", &opts.active_json_data))
        opts.active_json_data = DEFAULT_JSON_DATA;

    if (config_lookup_string(&config, "home_assistant_inactive_json_data", &opts.inactive_json_data))
        opts.inactive_json_data = DEFAULT_JSON_DATA;

    return opts;
}

void free_opts(struct options *opts)
{
    free(opts->home_assistant_active_url);
    free(opts->home_assistant_inactive_url);
    free((char *)opts->keyboard_device);
    free((char *)opts->mouse_device);
}