AM_CFLAGS = -D_DEFAULT_SOURCE -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L -std=c99 -W -Wall -O2 -I ../../../include/ -I ../../monikor/include
lib_LTLIBRARIES = network.la

network_la_SOURCES = network.c poll.c tcp.c
network_la_LDFLAGS = -module -avoid-version

networkconfdir = $(sysconfdir)/monikor/modules.d/
networkconf_DATA = ../../../etc/modules/network.yml
