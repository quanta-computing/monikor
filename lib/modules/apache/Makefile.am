AM_CFLAGS = -D_DEFAULT_SOURCE -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L -std=c99 -W -Wall -O2 -I ../../../include/ -I ../../monikor/include
lib_LTLIBRARIES = apache.la

apache_la_SOURCES = apache.c poll.c
apache_la_LDFLAGS = -module -avoid-version

apacheconfdir = $(sysconfdir)/monikor/modules.d/
apacheconf_DATA = ../../../etc/modules/apache.yml
