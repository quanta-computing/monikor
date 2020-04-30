AM_CFLAGS = -D_DEFAULT_SOURCE -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L -std=c99 -W -Wall -O2 -I ../../../include/ -I ../../monikor/include
lib_LTLIBRARIES = nginx.la

nginx_la_SOURCES = nginx.c poll.c
nginx_la_LDFLAGS = -module -avoid-version

nginxconfdir = $(sysconfdir)/monikor/modules.d/
nginxconf_DATA = ../../../etc/modules/nginx.yml
