// SPDX-License-Identifier: MIT

bin_PROGRAMS = hereweare
hereweare_SOURCES = main.c http_client.c options.c scan.c daemon.c log.c http_client.h options.h scan.h daemon.h log.h
hereweare_CFLAGS = -Wall -Werror $(AM_CFLAGS)
hereweare_LDADD = $(CURL_LIBS) $(SYSTEMD_LIBS) $(CONFIG_LIBS) $(LIBEVENT_LIBS) $(AM_LIBS)

