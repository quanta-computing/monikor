#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(quanta-agent, 1.2.2, support@quanta.io)
AM_INIT_AUTOMAKE([subdir-objects])
LT_INIT

AC_CONFIG_MACRO_DIR([m4])
# AC_CONFIG_SRCDIR(src/main.c)

AC_ENABLE_SHARED

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_LIBTOOL

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h inttypes.h netdb.h stddef.h stdint.h stdlib.h string.h sys/socket.h sys/statvfs.h sys/time.h syslog.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UID_T
AC_C_INLINE
AC_TYPE_MODE_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_CHOWN
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([dup2 gethostname getpagesize gettimeofday select socket strchr strdup strerror strndup strrchr strstr strtol strtoul strtoull])

# Check for libraries.
AC_CHECK_LIB([curl], [curl_easy_init])
AC_CHECK_LIB([dl], [dlopen])
AC_CHECK_LIB([yaml], [yaml_parser_initialize])

# Varnish version
AM_CONDITIONAL([VARNISH3], [pkg-config --atleast-version=3 varnishapi && ! pkg-config --atleast-version=4 varnishapi])
AM_CONDITIONAL([VARNISH4], [pkg-config --atleast-version=4 varnishapi && ! pkg-config --atleast-version=5 varnishapi])
AM_CONDITIONAL([VARNISH5], [pkg-config --atleast-version=5 varnishapi && ! pkg-config --atleast-version=5.2 varnishapi])
AM_CONDITIONAL([VARNISH6], [pkg-config --atleast-version=6 varnishapi && ! pkg-config --atleast-version=7 varnishapi])

AC_OUTPUT(src/Makefile lib/monikor/src/Makefile Makefile lib/modules/Makefile
  lib/modules/apache/Makefile
  lib/modules/cpu/Makefile
  lib/modules/disk/Makefile
  lib/modules/memcached/Makefile
  lib/modules/memory/Makefile
  lib/modules/mysqlstat/Makefile
  lib/modules/network/Makefile
  lib/modules/nfs/Makefile
  lib/modules/nginx/Makefile
  lib/modules/postgresql/Makefile
  lib/modules/process/Makefile
  lib/modules/redis/Makefile
  lib/modules/system/Makefile
  lib/modules/varnish3/Makefile
  lib/modules/varnish/Makefile
  lib/modules/varnish6/Makefile
  )
