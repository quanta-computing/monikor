AM_CFLAGS = -D_DEFAULT_SOURCE -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L -std=c99 -W -Wall -O2 -I ../../../include/ -I ../../monikor/include
lib_LTLIBRARIES = __NAME__.la

__NAME___la_SOURCES = __NAME__.c poll.c
__NAME___la_LDFLAGS = -module -avoid-version

__NAME__confdir = $(sysconfdir)/monikor/modules.d/
__NAME__conf_DATA = ../../../etc/modules/__NAME__.yml
