AM_CFLAGS = -D_DEFAULT_SOURCE -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L -std=c99 -W -Wall -O2 -I ../../../include/ -I ../../monikor/include
lib_LTLIBRARIES = disk.la

disk_la_SOURCES = disk.c poll.c
disk_la_LDFLAGS = -module -avoid-version

diskconfdir = $(sysconfdir)/monikor/modules.d/
diskconf_DATA = ../../../etc/modules/disk.yml
