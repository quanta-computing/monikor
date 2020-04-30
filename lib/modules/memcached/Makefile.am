AM_CFLAGS = -D_DEFAULT_SOURCE -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L -std=c99 -W -Wall -O2 -I ../../../include/ -I ../../monikor/include
lib_LTLIBRARIES = memcached.la

memcached_la_SOURCES = memcached.c poll.c
memcached_la_LDFLAGS = -module -avoid-version

memcachedconfdir = $(sysconfdir)/monikor/modules.d/
memcachedconf_DATA = ../../../etc/modules/memcached.yml
