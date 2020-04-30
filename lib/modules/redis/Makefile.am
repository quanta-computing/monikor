AM_CFLAGS = -D_DEFAULT_SOURCE -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L -std=c99 -W -Wall -O2 -I ../../../include/ -I ../../monikor/include
lib_LTLIBRARIES = redis.la

redis_la_SOURCES = redis.c poll.c
redis_la_LDFLAGS = -module -avoid-version

redisconfdir = $(sysconfdir)/monikor/modules.d/
redisconf_DATA = ../../../etc/modules/redis.yml
