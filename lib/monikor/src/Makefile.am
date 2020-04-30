AM_CFLAGS = -D_DEFAULT_SOURCE -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L -std=c99 -W -Wall -I../include
lib_LTLIBRARIES = libmonikor.la

libmonikor_la_SOURCES = metrics/metric.c \
			metrics/metric_list.c \
			metrics/metric_store.c \
			metrics/metric_compute.c \
			metrics/metric_list_serialize.c \
			metrics/metric_list_unserialize.c \
			utils/serialize.c

pkginclude_HEADERS = ../include/metric.h
pkgincludedir= $(includedir)/monikor
