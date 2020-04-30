AM_CFLAGS = -D_DEFAULT_SOURCE -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L -std=c99 -W -Wall -O2 -I ../../../include/ -I ../../monikor/include
lib_LTLIBRARIES = memory.la

memory_la_SOURCES = memory.c poll.c
memory_la_LDFLAGS = -module -avoid-version

memoryconfdir = $(sysconfdir)/monikor/modules.d/
memoryconf_DATA = ../../../etc/modules/memory.yml
