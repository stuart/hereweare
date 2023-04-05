SUBDIRS = src
dist_doc_DATA = README

AM_DISTCHECK_CONFIGURE_FLAGS = \
  --with-systemdsystemunitdir=$$dc_install_base/$(systemdsystemunitdir)

if HAVE_SYSTEMD
systemdsystemunit_DATA = hereweare.service
endif

EXTRA_DIST = hereweare.config