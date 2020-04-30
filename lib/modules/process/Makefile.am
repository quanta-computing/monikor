AM_CFLAGS = -D_DEFAULT_SOURCE -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L -std=c99 -W -Wall -O2 -I ../../../include/ -I ../../monikor/include
lib_LTLIBRARIES = process.la

process_la_SOURCES = process.c poll.c
process_la_LDFLAGS = -module -avoid-version

processconfdir = $(sysconfdir)/monikor/modules.d/
processconf_DATA = ../../../etc/modules/process.yml
