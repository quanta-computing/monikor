AM_CFLAGS = -D_DEFAULT_SOURCE -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L -std=c99 -W -Wall -O2 -I ../../../include/ -I ../../monikor/include
lib_LTLIBRARIES = cpu.la

cpu_la_SOURCES = cpu.c poll.c
cpu_la_LDFLAGS = -module -avoid-version

cpuconfdir = $(sysconfdir)/monikor/modules.d/
cpuconf_DATA = ../../../etc/modules/cpu.yml
