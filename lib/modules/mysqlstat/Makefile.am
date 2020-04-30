AM_CFLAGS = -D_DEFAULT_SOURCE -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L -std=c99 -W -Wall -O2 -I ../../../include/ -I ../../monikor/include
lib_LTLIBRARIES = mysqlstat.la

mysqlstat_la_SOURCES = mysqlstat.c poll.c
mysqlstat_la_LDFLAGS = -module -avoid-version
mysqlstat_la_LIBADD = -L/usr/lib64/mysql -lmysqlclient -lz -lm -ldl
mysqlstatconfdir = $(sysconfdir)/monikor/modules.d/
mysqlstatconf_DATA = ../../../etc/modules/mysqlstat.yml
