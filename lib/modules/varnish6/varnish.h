#ifndef varnish_H_
#define varnish_H_

#include <vapi/vsm.h>
#include <vapi/vsc.h>
#include "monikor.h"

#define MOD_NAME "varnish"
#define MONIKOR_VARNISH_MAX_INSTANCE_LENGTH 42

typedef struct {
  char *instance;
  monikor_t *mon;
  struct vsm *vsm;
  struct vsc *vsc;
} varnish_module_t;

typedef struct {
  varnish_module_t *mod;
  monikor_t *mon;
  struct timeval *clock;
  int count;
} varnish_iterator_data_t;


int varnish_poll_metrics(varnish_module_t *mod, struct timeval *clock);

#endif /* end of include guard: varnish_H_ */
