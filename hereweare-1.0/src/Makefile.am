
bin_PROGRAMS = hereweare
hereweare_SOURCES = main.c http_client.c options.c scan.c http_client.h options.h scan.h
hereweare_CFLAGS = -Wall -Werror $(AM_CFLAGS)
hereweare_LDADD = $(CURL_LIBS) $(SYSTEMD_LIBS) $(CONFIG_LIBS) $(AM_LIBS)

