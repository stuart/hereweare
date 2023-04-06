#pragma once

#ifdef HAVE_SYSTEMD
  #define HR_LOG_ERR(message, ...) fprintf(stderr, (message), ##__VA_ARGS__);

  #define HR_LOG_INFO(message, ...) printf((message), ##__VA_ARGS__);
#endif

#ifndef HAVE_SYSTEMD
#include <syslog.h>
  #define HR_LOG_ERR(message, ...) (syslog(LOG_ERR, (message), ##__VA_ARGS__))
  #define HR_LOG_INFO(message, ...) (syslog(LOG_INFO, (message), ##__VA_ARGS__))
#endif

void init_logging();