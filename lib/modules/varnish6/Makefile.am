AM_CFLAGS = -D_DEFAULT_SOURCE -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L -std=c99 -W -Wall -O2 -I ../../../include/ -I /usr/include/varnish -I ../../monikor/include
lib_LTLIBRARIES = varnish.la

varnish_la_SOURCES = varnish.c poll.c
varnish_la_LDFLAGS = -module -avoid-version
varnish_la_LIBADD = -lvarnishapi -ldl -lm

varnishconfdir = $(sysconfdir)/monikor/modules.d/
varnishconf_DATA = ../../../etc/modules/varnish.yml
