AM_CFLAGS = -D_DEFAULT_SOURCE -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L -std=c99 -W -Wall -O2 -I ../../../include/ -I ../../monikor/include
lib_LTLIBRARIES = nfs.la

nfs_la_SOURCES = nfs.c poll.c
nfs_la_LDFLAGS = -module -avoid-version

nfsconfdir = $(sysconfdir)/monikor/modules.d/
nfsconf_DATA = ../../../etc/modules/nfs.yml
