// SPDX-License-Identifier: MIT

#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "options.h"
#include "log.h"

/*
    Sends a HTTP message to the Home Assistant server url.
*/
int notify_home_assistant(struct options *opts, int active)
{
    CURL *curl = curl_easy_init();
    long http_code = 0;
    char error_buffer[CURL_ERROR_SIZE];

    if (curl)
    {
        CURLcode res;
        if (active == 1)
        {
            curl_easy_setopt(curl, CURLOPT_URL, opts->home_assistant_active_url);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, opts->active_json_data);
        }
        else
        {
            curl_easy_setopt(curl, CURLOPT_URL, opts->home_assistant_inactive_url);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, opts->inactive_json_data);
        }

        curl_easy_setopt(curl, CURLOPT_PORT, 8123);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charset: utf-8");
        headers = curl_slist_append(headers, opts->home_assistant_token);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        res = curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        if (res != CURLE_OK)
        {
            HR_LOG_ERR("HTTP Error: ");
            HR_LOG_ERR("%s\n", error_buffer);
        }
        curl_easy_cleanup(curl);

        return (res);
    }

    return (-1);
}

int activity_detected(struct options *opts)
{
    return (notify_home_assistant(opts, 1));
}

int inactivity_detected(struct options *opts)
{
    return (notify_home_assistant(opts, 0));
}
