AM_CFLAGS = -D_DEFAULT_SOURCE -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L -std=c99 -W -Wall -O2 -I ../../../include/ -I ../../monikor/include
lib_LTLIBRARIES = system.la

system_la_SOURCES = system.c poll.c
system_la_LDFLAGS = -module -avoid-version

systemconfdir = $(sysconfdir)/monikor/modules.d/
systemconf_DATA = ../../../etc/modules/system.yml
