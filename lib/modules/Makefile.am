AUTOMAKE_OPTIONS = foreign
SUBDIRS = apache \
  cpu \
  disk \
  memcached \
  memory \
  mysqlstat \
  network \
  nfs \
  nginx \
  postgresql \
  process \
  redis \
  system

if VARNISH3
  SUBDIRS += varnish3
endif
if VARNISH4
  SUBDIRS += varnish
endif
if VARNISH5
  SUBDIRS += varnish
endif
if VARNISH6
  SUBDIRS += varnish6
endif
